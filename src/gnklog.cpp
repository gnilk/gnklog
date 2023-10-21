//
// Created by gnilk on 19.10.23.
//

#include "gnklog.h"
#include "LogManager.h"

using namespace gnilk;

void Logger::Initialize() {
    LogManager::Instance().Initialize();
}

// New interface - returns shared ptr...
ILogger::Ref Logger::GetLogger(const std::string &name) {
    auto instance = LogManager::Instance().GetOrAddLogInstance(name);
    if (instance == nullptr) {
        return nullptr;
    }
    return instance->GetLog();
}

void Logger::AddSink(ILogOutputSink::Ref sink, const std::string &name) {
    LogManager::Instance().AddSink(sink, name);
}
void Logger::AddSink(ILogOutputSink *sink, const std::string &name) {
    LogManager::Instance().AddSink(sink, name);
}


bool Logger::RemoveSink(const std::string &name) {
    return LogManager::Instance().RemoveSink(name);
}



// This is the old interface - not encouraged...
ILogger* Logger::GetLoggerPtr(const std::string &name) {
    auto instance = GetLogger(name);
    if (instance == nullptr) {
        return nullptr;
    }
    return instance.get();
}


