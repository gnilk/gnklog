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

#include "logcore.h"
#include "logwriter.h"

namespace gnilk {


    class LogInstance {
    public:
        using Ref = std::shared_ptr<LogInstance>;
    public:
        static LogInstance::Ref Create(const std::string &name, ILogger::Ref loggerInstance) {
            // Note: using 'new' instead of make_shared since I want the CTOR protected!
            auto ptrInst = new LogInstance(name, std::move(loggerInstance));
            auto ref = std::shared_ptr<LogInstance>(ptrInst);
            return ref;
        }
        virtual ~LogInstance() = default;

        const std::string &GetName() {
            return logName;
        }
        ILogger::Ref GetLogger() {
            return logger;
        }

    protected:
        LogInstance(std::string name, ILogger::Ref loggerInstance) :
                logName(std::move(name)),
                logger(std::move(loggerInstance)) {

        }
    private:
        std::string logName = {};
        ILogger::Ref logger = {};
    };

    class LogManager {
    public:
        virtual ~LogManager() = default;
        static LogManager &Instance();
    public:
        LogInstance::Ref GetOrAddLogInstance(const std::string &name);

    private:
        LogManager() = default;

    protected:
        std::unordered_map<std::string, LogInstance::Ref> logInstances;

    };

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
