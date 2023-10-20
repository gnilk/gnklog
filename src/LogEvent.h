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

#pragma pack(push, 1)
        struct EventStreamMessage {
            uint8_t version = 0;
            LogTimeStamp  timeStamp = {};
            std::thread::id idSenderThread = {};
            LogLevel level = kDebug;
            char sender[LOG_MAX_NAME_LEN] = {};
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
        const std::string &String() const;

    protected:
        size_t Write();     // FIXME: Rename -> WriteIPC    ??
        size_t Read();      // FIXME: Rename -> ReadIPC     ??
        size_t ReadMsgString();

    private:
        void ComposeReportString();

    public:
        LogTimeStamp timeStamp;
        std::thread::id idSenderThread;
        std::string sender;
        LogLevel level = kDebug;
        std::string msgString;  // ONLY AVAILABLE ON AFTER READ

    private:
        std::string reportString;   // The resulting string after composition has been done, it's cached here

    };

}

#endif //GNKLOG_LOGEVENT_H
