//
// Created by gnilk on 19.10.23.
//

//
// !! Consider renaming this -> put it directly to ILogger??? !!
//
// This is the front-facing instance under the ILogger interface.
// When you call 'Debug' on this an event is created and pushed to a pipe.
//
// Then LogManager::SendToSinks is invoked which will determine where to send it and so forth..
//

#include <stdarg.h>
#include <optional>

#include "LogCore.h"
#include "LogWriter.h"
#include "LogManager.h"


using namespace gnilk;

ILogger::Ref LogWriter::Create(const std::string &logName) {
    auto ptrInst = new LogWriter(logName);
    ptrInst->Initialize();
    auto ref = std::shared_ptr<LogWriter>(ptrInst);
    return ref;
}

LogWriter::LogWriter(const std::string &logName) : name(logName) {

}

void LogWriter::Initialize() {
}

LogWriter::~LogWriter() {
}

/*
// public:
void LogWriter::WriteLine(const std::string &format, ...) {

}
void LogWriter::WriteLine(LogLevel logLevel, const std::string &format,...) {

}
void LogWriter::Critical(const std::string &format,...) {

}
void LogWriter::Error(const std::string &format, ...) {

}
void LogWriter::Warning(const std::string &format, ...) {

}
void LogWriter::Info(const std::string &format, ...) {

}
*/

FILE *LogWriter::GetWriteStream() {
    return LogManager::Instance().GetLogEventPipe().GetWriteStream();
}

void LogWriter::DebugRef(const char *format, ...) {
    va_list values;
    va_start( values, format);
    vfprintf(GetWriteStream(), format, values);
    va_end(    values);
}


 void LogWriter::GenerateEventData(LogEvent &outEvent, LogLevel level) {
    outEvent.timeStamp = LogClock::now();
    outEvent.level = level;
    outEvent.idSenderThread = std::this_thread::get_id();
    outEvent.sender = name;
    outEvent.Write();
}

void LogWriter::Debug(const char *format, ...) {
    va_list values;
    va_start( values, format);
    LogEvent logEvent;
    GenerateEventData(logEvent, kDebug);
    vfprintf(GetWriteStream(), format, values);
    va_end(    values);
    // Must flush data on the pipe for this to work...
    fflush(GetWriteStream());

    LogManager::Instance().SendToSinks();
}

