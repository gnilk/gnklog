//
// Created by gnilk on 19.10.23.
//

//
// This is the main logger class, it will dispatch LogEvent's to sinks...
//
#include <mutex>
#include <future>
#include <algorithm>
#include <deque>

#include "LogCore.h"
#include "LogManager.h"
#include "LogConsoleSink.h"

using namespace gnilk;


//
// Singleton - this way we enforce global registration of stuff - it is also thread safe
//
LogManager &LogManager::Instance() {
    static LogManager glbManager;
    glbManager.Initialize();
    return glbManager;
}

LogManager::~LogManager() {
    // event pipe will close through DTOR
}

// This will reset all data - mainly for unit-testing..
// NOTE: Any sink registered as unmanaged will be left untouched - i.e. the instance will be dangling unless
// the caller delete it!
void LogManager::Reset() {
    if (!isInitialized) {
        Initialize();
        return;
    }
    std::lock_guard<std::mutex> lock(instLock);
    cache->Clear();
    sinks.clear();
    logInstances.clear();

    // FIXME: this is not nice nor good...
    isInitialized = false;
    RegisterDefaultSinks();
    isInitialized = true;
}

//
// Initialize the logger
//
void LogManager::Initialize() {
    if (isInitialized) {
        return;
    }

    // IF we support some kind of configuration - except for compile time - we should deal with it here..
    // Option 2 - copy the 'property' object concept over from old logger and set all defaults in it - let the
    // app have the ability to supply/create/modify the Property object before we initialize...

    cache = LogCache::Create(cacheCapacity);

    RegisterDefaultSinks();

    if (!ipcHandler.Open()) {
        exit(1);
    }

    isInitialized = true;
}

//
// Register default sinks
//
void LogManager::RegisterDefaultSinks() {
    auto consoleSink = LogConsoleSink::Create();
    AddSink(consoleSink, "console");
}

//
// Returns an existing log-instance with a specific name or creates one if none is found
//
Log::Ref LogManager::GetOrAddLog(const std::string &name) {
    std::lock_guard<std::mutex> lock(instLock);

    if (logInstances.find(name) != logInstances.end()) {
        return logInstances[name]->GetLog();
    }
    // Create the writer and the instance...
    auto writer = Log::Create(name);
    auto instance = LogInstance::Create(name, writer);

    logInstances[name] = instance;
    return instance->GetLog();
}

Log::Ref LogManager::GetExistingLog(const std::string &name) {
    std::lock_guard<std::mutex> lock(instLock);
    if (logInstances.find(name) != logInstances.end()) {
        return logInstances[name]->GetLog();
    }
    return nullptr;
}

void LogManager::IterateLogs(const LogDelegate &cbInstance) {
    std::lock_guard<std::mutex> lock(instLock);
    for(auto [name, inst] : logInstances) {
        cbInstance(inst->GetLog());
    }
}

//
// Adds a managed sink to the list of sinks
//
void LogManager::AddSink(LogSink::Ref sink, const std::string &name) {
    std::lock_guard<std::mutex> lock(sinkLock);
    auto sinkInstance = LogSinkInstanceManaged::Create(sink, name);
    sinks.push_back(std::move(sinkInstance));
    // FIXME: This is not good - we can't do this during initialization
    if (isInitialized) {
        sink->OnAttached();
    }
}

//
// Adds an unmanaged sink to the list of sinks
// An unmanaged sink we have no control over - they can potentially go out of scope (be deleted and what not)
// while we operate on them.
//
void LogManager::AddSink(LogSink *sink, const std::string &name) {
    std::lock_guard<std::mutex> lock(sinkLock);
    auto sinkInstance = LogSinkInstanceUnmanaged::Create(sink, name);
    sinks.push_back(std::move(sinkInstance));
    // FIXME: This is not good - we can't do this during initialization
    if (isInitialized) {
        sink->OnAttached();
    }
}

//
// Remove a sink with a specific name, if not found we return false
//
bool LogManager::RemoveSink(const std::string &name) {
    std::lock_guard<std::mutex> lock(sinkLock);

    auto itSink = std::find_if(sinks.begin(), sinks.end(), [&name](const LogSinkInstance::Ref &other)->bool {
        return (other->GetName() == name);
    });

    if (itSink == sinks.end()) {
        return false;
    }
    sinks.erase(itSink);
    return true;
}

void LogManager::IterateSinks(const SinkDelegate &delegate) {
    std::lock_guard<std::mutex> lock(sinkLock);
    for(auto &sink : sinks) {
        delegate(sink->GetSink());
    }
}

void LogManager::IterateCache(const LogCache::CachedEventDelgate &delegate) {
    // FIXME: This should basically just forward to the log-cache - we just don't want to expose the cache to the outside world..
    cache->Iterate(delegate);
}

//
// This will forward all data to the log sinks
//
void LogManager::SendToSinks() {
    std::lock_guard<std::mutex> lock(sinkLock);

    // Fetch next event from the cache
    auto logEvent = cache->Next();

    auto &ipc = LogManager::Instance().GetIPC();

    if (ipc.ReadEvent(*logEvent) < 0) {
        // failed
        return;
    }
    logEvent->ComposeReportString();    // pre-compose the report string...

    std::deque<std::future<int>> sinkReadyList;

    for(auto &sinkInstance : sinks) {
        // This should not be problematic, except for maybe the unmanaged sink's which potentially could go out of
        // scope while we do this...
        auto sink = sinkInstance->GetSink();

        // Don't even send it unless it is within the range...
        if (!sink->WithinRange(logEvent->level)) {
            continue;
        }
        if (!sink->IsEnabled()) {
            continue;
        }

        auto future = std::async(&LogSink::Write, sink, *logEvent);
        sinkReadyList.push_back(std::move(future));
    }

    while(!sinkReadyList.empty()) {
        auto future = std::move(sinkReadyList.front());
        future.get();
        sinkReadyList.pop_front();
    }

}
