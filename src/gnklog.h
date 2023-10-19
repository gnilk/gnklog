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
