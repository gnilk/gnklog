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
        enum class kStatus {
            kOk = 0,
            kFiltered = 1,
            kSendError = 2,
        };
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
        inline kStatus Dbg(const std::string& format, T&&... args) {
            if (!(isEnabled && WithinRange(kDebug))) return kStatus::kFiltered;

            fmt::format_arg_store<fmt::format_context, T...> as{args...};
            auto str = fmt::vformat(format, as);

            return (SendLogMessage(kDebug, str) > 0) ? kStatus::kOk : kStatus::kSendError;
        }
        template <class...T>
        inline kStatus Inf(const std::string& format, T&&... args) {
            if (!(isEnabled && WithinRange(kInfo))) return kStatus::kFiltered;

            fmt::format_arg_store<fmt::format_context, T...> as{args...};
            auto str = fmt::vformat(format, as);

            return (SendLogMessage(kInfo, str) > 0) ? kStatus::kOk : kStatus::kSendError;
        }
        template <class...T>
        inline kStatus Warn(const std::string& format, T&&... args) {
            if (!(isEnabled && WithinRange(kWarning))) return kStatus::kFiltered;

            fmt::format_arg_store<fmt::format_context, T...> as{args...};
            auto str = fmt::vformat(format, as);

            return (SendLogMessage(kWarning, str) > 0) ? kStatus::kOk : kStatus::kSendError;
        }
        template <class...T>
        inline kStatus Err(const std::string& format, T&&... args) {
            if (!(isEnabled && WithinRange(kError))) return kStatus::kFiltered;

            fmt::format_arg_store<fmt::format_context, T...> as{args...};
            auto str = fmt::vformat(format, as);

            return (SendLogMessage(kError, str) > 0) ? kStatus::kOk : kStatus::kSendError;
        }
        template <class...T>
        inline kStatus Crit(const std::string& format, T&&... args) {
            if (!(isEnabled && WithinRange(kCritical))) return kStatus::kFiltered;

            fmt::format_arg_store<fmt::format_context, T...> as{args...};
            auto str = fmt::vformat(format, as);

            return (SendLogMessage(kCritical, str) > 0) ? kStatus::kOk : kStatus::kSendError;
        }

        //
        // these all use printf formatting - old legacy
        //
        template <class...T>
        inline kStatus Debug(T... arguments) const {
            if (!(isEnabled && WithinRange(kDebug))) return kStatus::kFiltered;
            auto str = fmt::sprintf(arguments...);
            return (SendLogMessage(kDebug, str) > 0) ? kStatus::kOk : kStatus::kSendError;

        }

        template <class...T>
        inline kStatus Info(T... arguments) const {
            if (!(isEnabled && WithinRange(kInfo))) return kStatus::kFiltered;
            auto str = fmt::sprintf(arguments...);
            return (SendLogMessage(kInfo, str) > 0) ? kStatus::kOk : kStatus::kSendError;
        }

        template <class...T>
        inline kStatus Warning(T... arguments) const {
            if (!(isEnabled && WithinRange(kWarning))) return kStatus::kFiltered;
            auto str = fmt::sprintf(arguments...);
            return (SendLogMessage(kWarning, str) > 0) ? kStatus::kOk : kStatus::kSendError;
        }

        template <class...T>
        inline kStatus Error(T... arguments) const {
            if (!(isEnabled && WithinRange(kError))) return kStatus::kFiltered;
            auto str = fmt::sprintf(arguments...);
            return (SendLogMessage(kError, str) > 0) ? kStatus::kOk : kStatus::kSendError;
        }

        template <class...T>
        inline kStatus Critical(T... arguments) const {
            if (!(isEnabled && WithinRange(kCritical))) return kStatus::kFiltered;
            auto str = fmt::sprintf(arguments...);
            return (SendLogMessage(kCritical, str) > 0) ? kStatus::kOk : kStatus::kSendError;
        }

    protected:
        int SendLogMessage(LogLevel level, const std::string &dbgMsg) const;

    protected:

        explicit Log(const std::string &logName);
        void Initialize();
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
