//
// Created by gnilk on 19.10.23.
//

#ifndef GNKLOG_LOGINSTANCE_H
#define GNKLOG_LOGINSTANCE_H

#include <memory>

#include "LogCore.h"

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
}

#endif //GNKLOG_LOGINSTANCE_H
