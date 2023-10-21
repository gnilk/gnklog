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

//
// Initialize the logger
//
void LogManager::Initialize() {
    if (isInitialized) {
        return;
    }
    RegisterDefaultSinks();

    if (!eventPipe.Open()) {
        exit(1);
    }

    isInitialized = true;
}

//
// Register default sinks
//
void LogManager::RegisterDefaultSinks() {
    auto consoleSink = LogConsoleSink::Create();
    AddSink(consoleSink);
}

//
// Returns an existing log-instance with a specific name or creates one if none is found
//
LogInstance::Ref LogManager::GetOrAddLogInstance(const std::string &name) {
    if (logInstances.find(name) != logInstances.end()) {
        return logInstances[name];
    }
    // Create the writer and the instance...
    auto writer = Log::Create(name);
    auto instance = LogInstance::Create(name, writer);

    logInstances[name] = instance;
    return instance;
    return nullptr;
}

//
// Adds a sink to the list of ACTIVE sinks
//
void LogManager::AddSink(ILogOutputSink::Ref sink) {
    // TODO: Wrap this up in a class 'LogSinkInstanceManager' that holds the ILogOutputSink
    //       Then have another 'LogInstanceUnmanaged' that just holds a pointer

    std::lock_guard<std::mutex> lock(sinkLock);
    auto sinkInstance = LogSinkInstanceManaged::Create(sink);
    sinks.push_back(std::move(sinkInstance));
}

void LogManager::AddSink(ILogOutputSink *sink) {
    std::lock_guard<std::mutex> lock(sinkLock);
    auto sinkInstance = LogSinkInstanceUnmanaged::Create(sink);
    sinks.push_back(std::move(sinkInstance));
}


//
// This will forward all data to the log sinks
//
void LogManager::SendToSinks() {
    std::lock_guard<std::mutex> lock(sinkLock);

    // Read and Compose the reporting string..
    LogEvent logEvent;
    logEvent.Read();
    logEvent.ComposeReportString();

    std::deque<std::future<int>> sinkReadyList;

    for(auto &sinkInstance : sinks) {
        // This should not be problematic, except for maybe the unmanaged sink's which potentially could go out of
        // scope while we do this...
        auto sink = sinkInstance->GetSink();
        auto future = std::async(&ILogOutputSink::Write, sink, logEvent);
        sinkReadyList.push_back(std::move(future));
    }

    while(!sinkReadyList.empty()) {
        auto future = std::move(sinkReadyList.front());
        future.get();
        sinkReadyList.pop_front();
    }

}
