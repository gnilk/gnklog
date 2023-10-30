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



void Log::SendLogMessage(LogLevel level, const std::string &dbgMsg) const {
    LogEvent logEvent(level);
    logEvent.timeStamp = LogClock::now();
    logEvent.idSenderThread = std::this_thread::get_id();
    logEvent.sender = name;

    auto ipc = LogManager::Instance().GetIPC();
    ipc->WriteEvent(logEvent, dbgMsg);


    // FIXME: should be removed
    //LogManager::Instance().SendToSinks();
}

