//
// Created by gnilk on 19.10.23.
//

#include <mutex>
#include <future>
#include <algorithm>
#include <fcntl.h>
#include <sys/stat.h>
#include <cstdio>
#include <unistd.h>
#include <filesystem>

#include "LogManager.h"
#include "LogConsoleSink.h"
#include "LogWriter.h"


using namespace gnilk;


LogManager &LogManager::Instance() {
    static LogManager glbManager;
    glbManager.Initialize();
    return glbManager;
}

LogManager::~LogManager() {
    // event pipe will close through DTOR
}


void LogManager::Initialize() {
    if (isInitialized) {
        return;
    }
    RegisterDefaultSinks();

    printf("Creating pies\n");
    if (!eventPipe.Open()) {
        exit(1);
    }

    isInitialized = true;
}

void LogManager::RegisterDefaultSinks() {
    auto consoleSink = LogConsoleSink::Create();
    AddSink(consoleSink);
}

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


void LogManager::AddSink(ILogOutputSink::Ref sink) {
    std::lock_guard<std::mutex> lock(sinkLock);
    sinks.push_back(sink);
}

void LogManager::SendToSinks() {
    std::lock_guard<std::mutex> lock(sinkLock);
    LogEvent logEvent;

    logEvent.Read();

    printf("Got Event!\n");
    for(auto &sink : sinks) {
        sink->Write(logEvent);
    }

/*
    auto future = std::async([this, &event]()-> void {
        for(auto &sink : sinks) {

        }
    });

    future.get();
*/
}
