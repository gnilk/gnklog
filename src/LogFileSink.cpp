//
// Created by gnilk on 25.10.23.
//

#include "LogFileSink.h"

using namespace gnilk;

LogFileSink::~LogFileSink() {
    Close();
}
void LogFileSink::Initialize(const std::vector<std::string_view> &args) {
    ParseArgs(args);
    OpenLogFile();
}

void LogFileSink::ParseArgs(const std::vector<std::string_view> &args) {
    for(size_t i=0;i<args.size();i++) {
        if (args[i] == "autoflush") {
            bFlushOnWrite = true;
        } else if (args[i] == "file") {
            filename = args[++i];
        } else if (args[i] == "append") {
            bAppend = true;
        } else {

        }
    }
}

void LogFileSink::OpenLogFile() {
    if (bAppend) {
        fOut = fopen(filename.c_str(), "a+");
    } else {
        fOut = fopen(filename.c_str(), "w+");
    }
}

LogSink::Ref LogFileSink::Create() {
    return std::make_shared<LogFileSink>();
}

int LogFileSink::Write(const LogEvent &logEvent) {
    if (fOut == nullptr) {
        return -1;
    }
    auto res = fprintf(fOut, "%s", logEvent.String().c_str());
    if (bFlushOnWrite) {
        fflush(fOut);
    }
    return res;
}

void LogFileSink::Flush() {
    if (fOut == nullptr) {
        return;
    }
    fflush(fOut);
}
void LogFileSink::Close() {
    if (fOut == nullptr) {
        return;
    }
    fclose(fOut);
    fOut = nullptr;
}

