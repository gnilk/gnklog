//
// Created by gnilk on 19.10.23.
//

#ifndef GNKLOG_LOGCORE_H
#define GNKLOG_LOGCORE_H

#include <memory>
#include <string>

namespace gnilk {
    enum LogLevel {
        kNone     = 0,
        kDebug    = 100,
        kInfo     = 200,
        kWarning  = 300,
        kError    = 400,
        kCritical = 500,
    };

    class ILogger {
    public:
        using Ref = std::shared_ptr<ILogger>;
    public:
        // functions
        virtual void WriteLine(const std::string &format,...) = 0;
        virtual void WriteLine(LogLevel logLevel, const std::string &format,...) = 0;

        virtual void Critical(const std::string &format,...) = 0;
        virtual void Error(const std::string &format, ...) = 0;
        virtual void Warning(const std::string &format, ...) = 0;
        virtual void Info(const std::string &format, ...) = 0;
        virtual void Debug(const std::string &format, ...) = 0;
    };

}

#endif //GNKLOG_LOGCORE_H
