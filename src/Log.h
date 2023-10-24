//
// Created by gnilk on 20.10.23.
//

#ifndef GNKLOG_LOG_H
#define GNKLOG_LOG_H

#include <memory>
#include <string>
#include <stdio.h>

#include "fmt/printf.h"
#include "fmt/format.h"

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

        void SetEnabled(bool newIsEnabled) {
            isEnabled = newIsEnabled;
        }
        bool IsEnabled() {
            return isEnabled;
        }

        void SetLogLevelThreshold(LogLevel newThreshold) {
            logLevelThreshold = newThreshold;
        }
        LogLevel GetLogLevelThreshold() const {
            return logLevelThreshold;
        }

        //
        // New and improved, using fmt - see: https://fmt.dev/latest/syntax.html
        //
        template <class...T>
        inline void Dbg(T... arguments) {
            if (!(isEnabled && WithinRange(kDebug))) return;
            auto str = fmt::format(arguments...);
            SendLogMessage(kDebug, str);
        }
        template <class...T>
        inline void Inf(T... arguments) {
            if (!(isEnabled && WithinRange(kInfo))) return;
            auto str = fmt::format(arguments...);
            SendLogMessage(kInfo, str);
        }
        template <class...T>
        inline void Warn(T... arguments) {
            if (!(isEnabled && WithinRange(kWarning))) return;
            auto str = fmt::format(arguments...);
            SendLogMessage(kWarning, str);
        }
        template <class...T>
        inline void Err(T... arguments) {
            if (!(isEnabled && WithinRange(kError))) return;
            auto str = fmt::format(arguments...);
            SendLogMessage(kError, str);
        }
        template <class...T>
        inline void Crit(T... arguments) {
            if (!(isEnabled && WithinRange(kCritical))) return;
            auto str = fmt::format(arguments...);
            SendLogMessage(kCritical, str);
        }

        //
        // these all use printf formatting - old legacy
        //
        template <class...T>
        inline void Debug(T... arguments) const {
            if (!(isEnabled && WithinRange(kDebug))) return;
            auto str = fmt::sprintf(arguments...);
            SendLogMessage(kDebug, str);
        }

        template <class...T>
        inline void Info(T... arguments) const {
            if (!(isEnabled && WithinRange(kInfo))) return;
            auto str = fmt::sprintf(arguments...);
            SendLogMessage(kInfo, str);
        }

        template <class...T>
        inline void Warning(T... arguments) const {
            if (!(isEnabled && WithinRange(kWarning))) return;
            auto str = fmt::sprintf(arguments...);
            SendLogMessage(kWarning, str);
        }

        template <class...T>
        inline void Error(T... arguments) const {
            if (!(isEnabled && WithinRange(kError))) return;
            auto str = fmt::sprintf(arguments...);
            SendLogMessage(kError, str);
        }

        template <class...T>
        inline void Critical(T... arguments) const {
            if (!(isEnabled && WithinRange(kCritical))) return;
            auto str = fmt::sprintf(arguments...);
            SendLogMessage(kCritical, str);
        }

    protected:
        void SendLogMessage(LogLevel level, const std::string &dbgMsg) const;

    protected:

        explicit Log(const std::string &logName);
        void Initialize();
        void GenerateAndSendEventData(LogEvent &outEvent, LogLevel level) const;
        __inline bool WithinRange(LogLevel msgLevel) const {
            return ((msgLevel >= logLevelThreshold)?true: false);
        }

    private:
        std::string name = {};
        bool isEnabled = true;
        LogLevel logLevelThreshold = LogLevel::kNone;   // Everything, no threshold
    };

    // Legacy - for now...
    using ILogger = Log;

}


#endif //GNKLOG_LOG_H
