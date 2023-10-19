//
// Created by gnilk on 19.10.23.
//

#include <string.h>

#include "LogEvent.h"
#include "LogManager.h"


using namespace gnilk;


size_t LogEvent::ToString(std::string &strOut) {
    if (reportString.empty()) {
        ComposeReportString();
    }
    strOut = reportString;

    return strOut.size();
}

static const std::string logLevelNames[] = {
    "NONE",             // 0
    "DEBUG",            // 1
    "INFO",             // 2
    "WARN",             // 3
    "ERROR",            // 4
    "CRITICAL"          // 5
};

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

void LogEvent::ComposeReportString() {
    char header[LOG_MAX_NAME_LEN + 64];
    char sTime[32];

    // a bit more fiddly
    auto msDuration = std::chrono::duration_cast<std::chrono::milliseconds>(timeStamp.time_since_epoch());
    auto msec = static_cast<int>(msDuration.count() % 1000);

    auto tstamp = LogClock::to_time_t(timeStamp);

    struct tm *gmt = std::gmtime(&tstamp);

    snprintf(sTime, 32, "%.4u-%.2u-%.2u %.2d:%.2d:%.2d.%.3d",
             gmt->tm_year + 1900, gmt->tm_mon + 1, gmt->tm_mday,
             gmt->tm_hour, gmt->tm_min, gmt->tm_sec, msec);


    static std::hash<std::thread::id> idHasher;

    uint32_t hashSenderThread = (idHasher(idSenderThread)) & (UINT32_MAX - 1);

    snprintf(header, LOG_MAX_NAME_LEN + 64, "%s [%.8x] %8s %32s - ",
             sTime,
             hashSenderThread,
             MessageClassNameFromInt(level).c_str(),
             sender.c_str());

    reportString = header;
    reportString += msgString;

    // FIXME: CRLN???
#ifdef LOG_HAVE_NEWLINE
    reportString += "\n";
#endif
}


// -------------------------------------------
// LogEvent
size_t LogEvent::Write() {

    LogEvent::EventStreamMessage msg;

    // Copy over everything to a message - for serialization
    msg.version = 0x01;
    msg.timeStamp = timeStamp;
    msg.idSenderThread = idSenderThread;
    msg.level = level;
    memset(msg.sender, 0, LOG_MAX_NAME_LEN);
    strncpy(msg.sender, sender.c_str(), LOG_MAX_NAME_LEN);

    // Write it over the event pipe..
    auto &eventPipe = LogManager::Instance().GetLogEventPipe();
    auto res = eventPipe.Write(&msg, sizeof(msg));

    if (res<0) {
        perror("LogEvent::Write");
        return 0;
    }

    return res;
}

// Read a log event from the pipe
size_t LogEvent::Read() {
    LogEvent::EventStreamMessage msg;
    auto &eventPipe = LogManager::Instance().GetLogEventPipe();

    auto res = eventPipe.Read(&msg, sizeof(msg));
    if (res < 0) {
        perror("LogEvent::Read");
        return 0;
    }

    timeStamp = msg.timeStamp;
    idSenderThread = msg.idSenderThread;
    sender = std::string(msg.sender);
    level = msg.level;

    if (!ReadMsgString()) {
        return 0;
    }

    printf("Reading Event, sz=%d\n", res);
    printf("  sender: %s\n", msg.sender);
    printf("  msg: %s\n", msgString.c_str());

    return res;
}

size_t LogEvent::ReadMsgString() {

    auto &eventPipe = LogManager::Instance().GetLogEventPipe();

    msgString.clear();

    int32_t res;
    uint8_t ch;

    while((res = eventPipe.Read(&ch, 1)) != 0) {
        if (res < 0) {
            perror("LogEvent::Read");
            return 0;
        }
        msgString += ch;
    }

    return msgString.length();
}

