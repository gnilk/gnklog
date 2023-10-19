//
// Created by gnilk on 19.10.23.
//

//
// simple stdout printf console sink...
//

#include <unistd.h>
#include <stdio.h>
#include "LogConsoleSink.h"

using namespace gnilk;

ILogOutputSink::Ref LogConsoleSink::Create() {
    auto sink = std::make_shared<LogConsoleSink>();
    return sink;
}

int LogConsoleSink::Write(LogEvent &logEvent) {

    std::string logString;
    logEvent.ToString(logString);
    fprintf(stdout, "%s", logString.c_str());

    return 1;
}
