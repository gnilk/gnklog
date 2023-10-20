//
// Created by gnilk on 19.10.23.
//

#ifndef GNKLOG_LOGINSTANCE_H
#define GNKLOG_LOGINSTANCE_H

#include <memory>

#include "Log.h"
#include "LogCore.h"

namespace gnilk {
    class LogInstance {
    public:
        using Ref = std::shared_ptr<LogInstance>;
    public:
        static LogInstance::Ref Create(const std::string &name, Log::Ref logInstance) {
            // Note: using 'new' instead of make_shared since I want the CTOR protected!
            auto ptrInst = new LogInstance(name, std::move(logInstance));
            auto ref = std::shared_ptr<LogInstance>(ptrInst);
            return ref;
        }
        virtual ~LogInstance() = default;

        const std::string &GetName() {
            return logName;
        }

        Log::Ref GetLog() {
            return logger;
        }

    protected:
        LogInstance(std::string name, Log::Ref logInstance) :
                logName(std::move(name)),
                logger(std::move(logInstance)) {

        }
    private:
        std::string logName = {};
        Log::Ref logger = {};
    };
}

#endif //GNKLOG_LOGINSTANCE_H
