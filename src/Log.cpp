//
// Created by gnilk on 20.10.23.
//
#include <stdarg.h>

#include "Log.h"
#include "LogManager.h"

using namespace gnilk;

Log::Log(const std::string &logName) : name(logName) {

}

void Log::Initialize() {

}

Log::~Log() {

}

Log::Ref Log::Create(const std::string &logName) {
    auto ptrInst = new Log(logName);
    ptrInst->Initialize();
    auto ref = std::shared_ptr<Log>(ptrInst);
    return ref;
}


void Log::GenerateAndSendEventData(LogEvent &outEvent, LogLevel level) const {
    outEvent.timeStamp = LogClock::now();
    outEvent.level = level;
    outEvent.idSenderThread = std::this_thread::get_id();
    outEvent.sender = name;
    outEvent.Write();
}

void Log::SendLogMessage(LogLevel level, const std::string &dbgMsg) const {
    LogEvent logEvent;
    GenerateAndSendEventData(logEvent, level);
    logEvent.WriteMsgString(dbgMsg);
    LogManager::Instance().SendToSinks();
}

