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
ILogger* Logger::GetLogger(const std::string &name) {
    auto instance = GetLoggerRef(name);
    if (instance == nullptr) {
        return nullptr;
    }
    return instance.get();
}
// This is the old interface - not encouraged...
Log::Ref Logger::GetLoggerRef(const std::string &name) {
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
       return true;
    });
}

void Logger::DisableAllLoggers() {
    LogManager::Instance().IterateLogs([](const Log::Ref &log) {
        log->SetEnabled(false);
        return true;
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
    LogManager::Instance().IterateSinks([newDebugLevel](LogSink *sink) {
        sink->SetLogLevelThreshold(newDebugLevel);
        return true;
    });
}

void Logger::DisableSink(const std::string &name) {
    LogManager::Instance().IterateSinks([&name](LogSink *sink) {
        if (name == sink->GetName()) {
            sink->SetEnabled(false);
            return false;
        }
        return true;
    });
}

void Logger::EnableSink(const std::string &name) {
    LogManager::Instance().IterateSinks([&name](LogSink *sink) {
        if (name == sink->GetName()) {
            sink->SetEnabled(true);
            return false;
        }
        return true;
    });
}

void Logger::DisableAllSinks() {
    LogManager::Instance().IterateSinks([](LogSink *sink) {
        sink->SetEnabled(false);
        return true;
    });
}

void Logger::EnableAllSinks() {
    LogManager::Instance().IterateSinks([](LogSink *sink) {
        sink->SetEnabled(true);
        return true;
    });
}


