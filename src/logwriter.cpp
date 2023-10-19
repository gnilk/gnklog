//
// Created by gnilk on 19.10.23.
//
#include "logcore.h"
#include "logwriter.h"

using namespace gnilk;

ILogger::Ref LogWriter::Create(const std::string &logName) {
    auto ptrInst = new LogWriter(logName);
    auto ref = std::shared_ptr<LogWriter>(ptrInst);
    return ref;
}

LogWriter::LogWriter(const std::string &logName) : name(logName) {

}


// public:
void LogWriter::WriteLine(const std::string &format,...) {

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
void LogWriter::Debug(const std::string &format, ...) {

}

