//
// Created by gnilk on 19.10.23.
//

#ifndef GNKLOG_LOGCORE_H
#define GNKLOG_LOGCORE_H

#include <memory>
#include <chrono>
#include <vector>
#include <string>

namespace gnilk {

    // The max name of a logger instance
#ifndef LOG_MAX_NAME_LEN
    #define LOG_MAX_NAME_LEN 64
#endif

    using LogClock = std::chrono::system_clock;
    using LogTimeStamp = std::chrono::time_point<LogClock>;

    enum LogLevel {
        kNone     = 0,
        kDebug    = 100,
        kInfo     = 200,
        kWarning  = 300,
        kError    = 400,
        kCritical = 500,
        kCustom   = 600,
    };

    // The actual 'logger' is implemented by 'LogWriter' - very stupid, indeed...
    class ILogger {
    public:
        using Ref = std::shared_ptr<ILogger>;
    public:
        virtual void Debug(const char *format, ...) = 0;
        virtual void DebugRef(const char *format, ...) = 0;
    };

}

#endif //GNKLOG_LOGCORE_H
