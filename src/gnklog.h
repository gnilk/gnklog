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

namespace gnilk {

    // This mimics to some extent the old logger
    // All 'core' functionality has been moved around.
    // The 'LogManager' (lack of better name for now) is the hidden internal core - does all the heavy lifting.
    class Logger {
    public:
        Logger() = default;
        virtual ~Logger() = default;

        static void Initialize();

        // Log functionality
        static ILogger::Ref GetLogger(const std::string &name);
        static ILogger *GetLoggerPtr(const std::string &name);
        static void DisableLogger(const std::string &name);
        static void EnableLogger(const std::string &name);
        static void DisableAllLoggers();
        static void EnableAllLoggers();


        // Sink functionality
        static void AddSink(LogSink::Ref sink, const std::string &name);
        static void AddSink(LogSink *sink, const std::string &name);
        static bool RemoveSink(const std::string &name);
        static void SetAllSinkDebugLevel(LogLevel newDebugLevel);


    };

}

#endif //GNKLOG_GNKLOG_H
