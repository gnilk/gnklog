//
// Created by gnilk on 19.10.23.
//

#ifndef GNKLOG_LOGMANAGER_H
#define GNKLOG_LOGMANAGER_H

#include <unordered_map>
#include <memory>
#include <string>
#include <mutex>
#include <functional>

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
        Log::Ref GetOrAddLog(const std::string &name);
        Log::Ref GetExistingLog(const std::string &name);
        void IterateLogs(const std::function<void(const Log::Ref &)> &);

        void RegisterDefaultSinks();
        void AddSink(LogSink::Ref sink, const std::string &name);
        void AddSink(LogSink *sink, const std::string &name);
        bool RemoveSink(const std::string &name);
        void IterateSinks(const std::function<void(const LogSink *)> &);

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

        std::mutex instLock;
        std::mutex sinkLock;
        std::unordered_map<std::string, LogInstance::Ref> logInstances;
        std::vector<LogSinkInstance::Ref> sinks;

        int eventMsgWritePipe = -1;
        int eventMsgReadPipe = -1;

    };

}

#endif //GNKLOG_LOGMANAGER_H
