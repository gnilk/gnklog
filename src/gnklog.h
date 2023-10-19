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

#include "LogCore.h"

namespace gnilk {

    // This mimics to some extent the old logger
    // All 'core' functionality has been moved around.
    // The 'LogManager' (lack of better name for now) is the hidden internal core - does all the heavy lifting.
    class Logger {
    public:
        Logger() = default;
        virtual ~Logger() = default;

        static void Initialize();
        static ILogger::Ref GetLogger(const std::string &name);
        static ILogger *GetLoggerPtr(const std::string &name);

    };

}

#endif //GNKLOG_GNKLOG_H
