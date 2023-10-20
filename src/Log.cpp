//
// Created by gnilk on 20.10.23.
//
#include <stdarg.h>

#include "Log.h"
#include "LogManager.h"

using namespace gnilk;

Log::Ref Log::Create(const std::string &logName) {
    auto ptrInst = new Log(logName);
    ptrInst->Initialize();
    auto ref = std::shared_ptr<Log>(ptrInst);
    return ref;
}

Log::Log(const std::string &logName) : name(logName) {

}

void Log::Initialize() {
}

Log::~Log() {
}

/*
// public:
void Log::WriteLine(const std::string &format, ...) {

}
void Log::WriteLine(LogLevel logLevel, const std::string &format,...) {

}
void Log::Critical(const std::string &format,...) {

}
void Log::Error(const std::string &format, ...) {

}
void Log::Warning(const std::string &format, ...) {

}
void Log::Info(const std::string &format, ...) {

}
*/

FILE *Log::GetWriteStream() {
    return LogManager::Instance().GetLogEventPipe().GetWriteStream();
}



void Log::GenerateEventData(LogEvent &outEvent, LogLevel level) {
    outEvent.timeStamp = LogClock::now();
    outEvent.level = level;
    outEvent.idSenderThread = std::this_thread::get_id();
    outEvent.sender = name;
    outEvent.Write();
}

// This is not supported (va_start can't take reference) - however, GCC + Clang - correctly compiles this but MSVC throws an error
void Log::DebugRef(const std::string &format, ...) {
    va_list values;
    LogEvent logEvent;
    GenerateEventData(logEvent, kDebug);

    va_start( values, format);
    vfprintf(GetWriteStream(), format.c_str(), values);
    va_end(    values);

    // Must flush data on the pipe for this to work...
    fflush(GetWriteStream());
    LogManager::Instance().SendToSinks();
}

void Log::Debug(const char *format, ...) {
    va_list values;
    LogEvent logEvent;
    GenerateEventData(logEvent, kDebug);

    va_start( values, format);
    vfprintf(GetWriteStream(), format, values);
    va_end(    values);

    // Must flush data on the pipe for this to work...
    fflush(GetWriteStream());
    LogManager::Instance().SendToSinks();
}

