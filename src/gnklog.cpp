//
// Created by gnilk on 19.10.23.
//

#include "gnklog.h"

using namespace gnilk;

void Logger::Initialize() {

}

// New interface - returns shared ptr...
ILogger::Ref GetLogger(const std::string &name) {
    auto instance = LogManager::Instance().GetOrAddLogInstance(name);
    if (instance == nullptr) {
        return nullptr;
    }
    return instance->GetLogger();
}

// This is the old interface - not encouraged...
ILogger* Logger::GetLoggerPtr(const std::string &name) {
    auto instance = GetLogger(name);
    if (instance == nullptr) {
        return nullptr;
    }
    return instance.get();
}


LogManager &LogManager::Instance() {
    static LogManager glbManager;
    return glbManager;
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


