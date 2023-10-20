//
// Created by gnilk on 20.10.23.
//

#ifndef GNKLOG_LOG_H
#define GNKLOG_LOG_H

#include <memory>
#include <string>
#include <stdio.h>

#include "LogCore.h"
#include "LogEvent.h"

namespace gnilk {
    // This a Log
    class Log {
    public:
        using Ref = std::shared_ptr<Log>;
    public:
        static Log::Ref Create(const std::string &logName);
        virtual ~Log();

        void DebugRef(const std::string &format, ...);
        void Debug(const char *format, ...);
    protected:
        explicit Log(const std::string &logName);
        void Initialize();
        void GenerateEventData(LogEvent &outEvent, LogLevel level);
        FILE *GetWriteStream();
    private:
        std::string name;
    };

    // Legacy - for now...
    using ILogger = Log;

}


#endif //GNKLOG_LOG_H
