//
// Created by gnilk on 19.10.23.
//

#ifndef GNKLOG_LOGMANAGER_H
#define GNKLOG_LOGMANAGER_H

#include <unordered_map>
#include <memory>
#include <string>
#include <mutex>

#include "LogCore.h"
#include "LogEventFifoUnix.h"
#include "LogEventPipeUnix.h"
#include "LogInstance.h"
#include "LogInternal.h"
#include "LogSink.h"

namespace gnilk {

    class LogManager {
    public:
        virtual ~LogManager();
        static LogManager &Instance();

    public:
        void Initialize();
        LogInstance::Ref GetOrAddLogInstance(const std::string &name);

        void RegisterDefaultSinks();
        void AddSink(ILogOutputSink::Ref sink);
        void AddSink(ILogOutputSink *sink);

        LogIPCBase &GetLogEventPipe() {
            return eventPipe;
        }

        void SendToSinks();

    private:
        LogManager() = default;

    protected:
        bool isInitialized = false;


        LogEventPipeUnix eventPipe;
//        LogEventFifoUnix eventPipe;

        std::mutex sinkLock;
        std::unordered_map<std::string, LogInstance::Ref> logInstances;
        std::vector<LogSinkInstance::Ref> sinks;

        int eventMsgWritePipe = -1;
        int eventMsgReadPipe = -1;

    };

}

#endif //GNKLOG_LOGMANAGER_H
