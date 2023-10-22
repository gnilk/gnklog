//
// Created by gnilk on 19.10.23.
//

#ifndef GNKLOG_LOGSINK_H
#define GNKLOG_LOGSINK_H

#include <string>
#include <vector>
#include <memory>

#include "LogEvent.h"



namespace gnilk {
    class LogManager;

    class LogSink {
        friend LogManager;
    public:
        using Ref = std::shared_ptr<LogSink>;
    public:
        LogSink() = default;
        explicit LogSink(const std::string &sinkName) : name(sinkName) {}
        virtual ~LogSink() = default;

        const std::string &GetName() {
            return name;
        };

        LogLevel GetLogLevelThreshold() {
            return logLevelThreshold;
        }
        void SetLogLevelThreshold(LogLevel newThreshold) {
            logLevelThreshold = newThreshold;
        }

        virtual void Initialize([[maybe_unused]] const std::vector<const std::string &> &args) {

        }
        virtual bool Open() {
            return false;
        };
        virtual int Write(const LogEvent &logEvent) {
            return -1;
        };
        virtual void Flush() {  };
        virtual void Close() {  };
    protected:
        __inline bool WithinRange(LogLevel msgLevel) {
            return ((msgLevel >= logLevelThreshold)?true: false);
        }
    private:
        const std::string name;
        LogLevel logLevelThreshold = kNone;
    };
    // legacy compatibility
    using LogBaseSink = LogSink;
    using ILogOutputSink = LogSink;

}

#endif //GNKLOG_LOGSINK_H
