//
// Created by gnilk on 19.10.23.
//

//
// simple stdout printf console sink...
//

#ifndef WIN32
#include <unistd.h>
#endif

#include <stdio.h>
#include "LogConsoleSink.h"

using namespace gnilk;

LogSink::Ref LogConsoleSink::Create() {
    auto sink = std::make_shared<LogConsoleSink>();
    return sink;
}

int LogConsoleSink::Write(const LogEvent &logEvent) {

    auto &logString = logEvent.String();
    fprintf(stdout, "%s", logString.c_str());

    return 1;
}
