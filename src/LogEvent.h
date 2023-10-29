//
// Created by gnilk on 19.10.23.
//

#ifndef GNKLOG_LOGEVENT_H
#define GNKLOG_LOGEVENT_H

#include <stdint.h>
#include <thread>
#include <string>
#include <chrono>

#include "LogCore.h"

namespace gnilk {
    class Log;
    class LogManager;

    class LogEvent {
        friend Log;
        friend LogManager;
    protected:

        //
        // This struct is used to pass the LogEvent between the loggers and the sink handling
        // The LogEvent must be passed as an atomic operation - single write - thus we create a buffer
        // to hold the full event...
        //
#pragma pack(push, 1)
        struct EventStreamMessage {
            uint8_t version = 0;
            LogTimeStamp  timeStamp = {};
            std::thread::id idSenderThread = {};
            LogLevel level = kDebug;
            char sender[LOG_MAX_NAME_LEN] = {};
            uint16_t messageLength = 0;
            // Message follows - the buffer is allocated including the message length
        };
#pragma pack(pop)

    public:
        LogEvent() = default;
        explicit LogEvent(LogLevel lvl) : level(lvl) {}
        virtual ~LogEvent() = default;

        LogLevel Level() const {
            return level;
        }

        size_t ToString(std::string &strOut);

        //
        // Returns a const-ref to the report string (header + msg) - DO NOT change const of this string..
        //
        __inline const std::string &String() const {
            return reportString;
        }

        //
        // Returns the header (time, thread, logger, level)
        //
        __inline const std::string &HeaderString() const {
            return headerString;
        }

        //
        // Returns the debug msg without the header
        //
        __inline const std::string &MsgString() const {
            return msgString;
        }

    protected:
        size_t Write(const std::string &dbgMessage);
        size_t Read();      // FIXME: Rename -> ReadIPC     ??
//        size_t WriteMsgString(const char *str);
//        size_t WriteMsgString(const std::string &str);
//        size_t ReadMsgString();

    private:
        void ComposeReportString();

    public:
        LogTimeStamp timeStamp;
        std::thread::id idSenderThread;
        std::string sender;
        LogLevel level = kDebug;
        std::string msgString;  // ONLY AVAILABLE ON AFTER READ

    private:
        std::string headerString;

        std::string reportString;   // The resulting string after composition has been done, it's cached here

    };

}

#endif //GNKLOG_LOGEVENT_H
