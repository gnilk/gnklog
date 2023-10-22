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
    return LogManager::Instance().GetOrAddLog(name);
}

void Logger::DisableLogger(const std::string &name) {
    auto log = LogManager::Instance().GetExistingLog(name);
    if (log == nullptr) {
        return;
    }

    log->SetEnabled(false);
}
void Logger::EnableLogger(const std::string &name) {
    auto log = LogManager::Instance().GetExistingLog(name);
    if (log == nullptr) {
        return;
    }

    log->SetEnabled(false);
}

void Logger::EnableAllLoggers() {
    LogManager::Instance().IterateLogs([](const Log::Ref &log) {
       log->SetEnabled(true);
    });
}

void Logger::DisableAllLoggers() {
    LogManager::Instance().IterateLogs([](const Log::Ref &log) {
        log->SetEnabled(false);
    });
}


void Logger::AddSink(LogSink::Ref sink, const std::string &name) {
    LogManager::Instance().AddSink(sink, name);
}
void Logger::AddSink(LogSink *sink, const std::string &name) {
    LogManager::Instance().AddSink(sink, name);
}


bool Logger::RemoveSink(const std::string &name) {
    return LogManager::Instance().RemoveSink(name);
}

void Logger::SetAllSinkDebugLevel(LogLevel newDebugLevel) {
    LogManager::Instance().IterateSinks([newDebugLevel](const LogSink *sink) {

    });
}

// This is the old interface - not encouraged...
ILogger* Logger::GetLoggerPtr(const std::string &name) {
    auto instance = GetLogger(name);
    if (instance == nullptr) {
        return nullptr;
    }
    return instance.get();
}


