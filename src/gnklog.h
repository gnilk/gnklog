//
// Created by gnilk on 19.10.23.
//

#ifndef GNILK_GNKLOG_H
#define GNILK_GNKLOG_H

#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <unordered_map>

// Bring in the main - external exposing files
#include "Log.h"
#include "LogCore.h"
#include "LogEvent.h"
#include "LogSink.h"
#include "LogConsoleSink.h"
#include "LogFileSink.h"

namespace gnilk {

    // This mimics to some extent the old logger
    // All 'core' functionality has been moved around.
    // The 'LogManager' (lack of better name for now) is the hidden internal core - does all the heavy lifting.
    class Logger {
    public:
        using ILogger = Log;
    public:
        Logger() = default;
        virtual ~Logger() = default;

        static void Initialize();

        // FIXME: I should really look at these..
        static ILogger* GetLogger(const std::string &name);
        static Log::Ref GetLoggerRef(const std::string &name);

        static void DisableLogger(const std::string &name);
        static void EnableLogger(const std::string &name);
        static void DisableAllLoggers();
        static void EnableAllLoggers();


        // Sink functionality
        static void AddSink(LogSink::Ref sink, const std::string &name);
        static void AddSink(LogSink *sink, const std::string &name);
        static bool RemoveSink(const std::string &name);
        static void SetAllSinkDebugLevel(LogLevel newDebugLevel);
        static void DisableSink(const std::string &name);
        static void EnableSink(const std::string &name);
        static void DisableAllSinks();
        static void EnableAllSinks();


    };

}

#endif //GNKLOG_GNKLOG_H
