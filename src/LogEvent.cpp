//
// Created by gnilk on 19.10.23.
//

//
// The LogEvent class encapsulate the internals of what happens between a call to 'logger->Debug("hello")' and the console
//

#include <string.h>

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

    headerString = header;


    reportString = headerString + msgString;

#ifdef LOG_HAVE_NEWLINE
    reportString += "\n";
#endif
}

template<typename T>
T ptrOffset(void *basePtr, int32_t offset) {
    static_assert(std::is_pointer<T>());
    return (T )(reinterpret_cast<unsigned char *>(basePtr) + offset);
}

//
// Many will call 'Write' but only one will call read - so it is important that the
// write is atomic in nature..
//
size_t LogEvent::Write(const std::string &dbgMessage) {



    // This the amount we need for the complete message, including string and terminating zero
    size_t nBytesTotal = sizeof(LogEvent::EventStreamMessage) + dbgMessage.length() + 1;

    // This is the buffer holding the serialized event + message
    auto ptrBuffer = alloca(nBytesTotal);
    if (ptrBuffer == nullptr) {
        return 0;
    }
    // Reset the buffer
    memset(ptrBuffer, 0, nBytesTotal);
    auto msg = static_cast<LogEvent::EventStreamMessage *>(ptrBuffer);

    // Copy over everything to a message - for serialization
    msg->version = 0x01;
    msg->timeStamp = timeStamp;
    msg->idSenderThread = idSenderThread;
    msg->level = level;
    msg->messageLength = dbgMessage.size();
    strncpy(msg->sender, sender.c_str(), LOG_MAX_NAME_LEN);

    auto ptrString = ptrOffset<char *>(ptrBuffer, sizeof(LogEvent::EventStreamMessage));
    strncpy(ptrString, dbgMessage.c_str(), dbgMessage.size());

    // Write it over the event pipe..
    auto &eventPipe = LogManager::Instance().GetLogEventPipe();
    //auto res = eventPipe.Write(&msg, sizeof(msg));
    auto res = eventPipe.Write(ptrBuffer, nBytesTotal);

    if (res<0) {
        perror("LogEvent::Write::write");
        return 0;
    }
    return res;
}

//
// Reads a log event from the pipe
//
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

    auto msgBuffer = (char *)alloca(msg.messageLength+1);
    memset(msgBuffer, 0, msg.messageLength + 1);
    int res2 = eventPipe.Read(msgBuffer, msg.messageLength + 1);
    if (res2 < 0) {
        return -1;
    }
    res += res2;

    msgString = std::string(msgBuffer);

    return res;
}

//
// Reads the actual formatted string off the pipe..
//
//size_t LogEvent::ReadMsgString() {
//
//    auto &eventPipe = LogManager::Instance().GetLogEventPipe();
//
//    msgString.clear();
//
//    int32_t res;
//    uint8_t ch;
//
//    while((res = eventPipe.Read(&ch, 1)) != 0) {
//        if (res < 0) {
//            perror("LogEvent::Read");
//            return 0;
//        }
//        msgString += ch;
//    }
//
//    return msgString.length();
//}

