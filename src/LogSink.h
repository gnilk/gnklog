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

        // Default impl. just filters on the range and forwards to normal write - this makes cached event handling
        // work for most use-cases without custom sink's needing to do anything...
        virtual bool WriteCachedEvent(const LogEvent &logEvent) {
            if (WithinRange(logEvent.level)) {
                if (Write(logEvent) < 0) return false;
            }
            return true;
        }
        virtual void Flush() {  };
        virtual void Close() {  };
    protected:
        //
        // We got attached, this will require a bit more logic..
        // Should this be here??
        //
        virtual void OnAttached();

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
