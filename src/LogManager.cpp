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

#include "LogManager.h"
#include "LogConsoleSink.h"
#include "LogWriter.h"


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
    auto writer = LogWriter::Create(name);
    auto instance = LogInstance::Create(name, writer);

    logInstances[name] = instance;
    return instance;
    return nullptr;
}

//
// Adds a sink to the list of ACTIVE sinks
//
void LogManager::AddSink(ILogOutputSink::Ref sink) {
    std::lock_guard<std::mutex> lock(sinkLock);
    sinks.push_back(sink);
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

    for(auto &sink : sinks) {
        auto future = std::async(&ILogOutputSink::Write, sink, logEvent);
        sinkReadyList.push_back(std::move(future));
    }

    while(!sinkReadyList.empty()) {
        auto future = std::move(sinkReadyList.front());
        future.get();
        sinkReadyList.pop_front();
    }

}
