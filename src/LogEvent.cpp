//
// Created by gnilk on 19.10.23.
//

//
// The LogEvent class encapsulate the internals of what happens between a call to 'logger->Debug("hello")' and the console
//

#include <string.h>

#include "fmt/format.h"
#include "LogEvent.h"
#include "LogManager.h"


using namespace gnilk;


//
// This will create the complete report string, cache it and return it.
// In case there are several sinks - ok, I have never used more than two - but anyway - I like the ambition...
//
size_t LogEvent::ToString(std::string &strOut) {
    if (reportString.empty()) {
        ComposeReportString();
    }
    strOut = reportString;

    return strOut.size();
}

//
// fancy lookup table...
//
static const std::string logLevelNames[] = {
    "NONE",             // 0
    "DEBUG",            // 1
    "INFO",             // 2
    "WARN",             // 3
    "ERROR",            // 4
    "CRITICAL"          // 5
};

//
// helper to fetch the right name from the level
//
static const std::string &MessageClassNameFromInt(LogLevel level) {
    if (level < (int) kDebug) {
        return logLevelNames[0];
    } else if (level < (int) kInfo) {
        return logLevelNames[1];
    } else if (level < (int) kWarning) {
        return logLevelNames[2];
    } else if (level < (int) kError) {
        return logLevelNames[3];
    } else if (level < (int) kCritical) {
        return logLevelNames[4];
    }
    return logLevelNames[5];
}

//
// Compose a report string from an event
// Consider removing the composer out of here...
//
void LogEvent::ComposeReportString() {
    char sTime[32];

    // a bit more fiddly
    auto msDuration = std::chrono::duration_cast<std::chrono::milliseconds>(timeStamp.time_since_epoch());
    auto msec = static_cast<int>(msDuration.count() % 1000);

    auto tstamp = LogClock::to_time_t(timeStamp);

    struct tm *gmt = std::gmtime(&tstamp);

    // sTime = 22 chars
    // FIXME: redo this with fmt
    snprintf(sTime, 32, "%.4u-%.2u-%.2u %.2d:%.2d:%.2d.%.3d",
             gmt->tm_year + 1900, gmt->tm_mon + 1, gmt->tm_mday,
             gmt->tm_hour, gmt->tm_min, gmt->tm_sec, msec);


    static std::hash<std::thread::id> tidHasher;

    uint32_t hashSenderThread = (tidHasher(idSenderThread)) & (UINT32_MAX - 1);
    uint32_t hashSenderProc = idSenderProc & (UINT32_MAX -1);

    // Allow some kind of config around this...
    // FIXME: we should check 'FMT_EXCEPTIONS' here
    //        if enabled, libfmt can throw exceptions and we don't want that...
    try {
        headerString = fmt::format("{} [{:#010x}:{:#010x}] {:8} {:32}",
                                   sTime,
                                   hashSenderProc,
                                   hashSenderThread,
                                   MessageClassNameFromInt(level),
                                   sender);
    } catch(fmt::format_error &e) {
        fprintf(stderr, "fmt exception; ", e.what());
        reportString = e.what();
        return;
    }

    reportString = headerString + msgString;

#ifdef LOG_HAVE_NEWLINE
    reportString += "\n";
#endif
}


