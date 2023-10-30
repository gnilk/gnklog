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
#include "LogCache.h"
#include "LogIPCFifoUnix.h"
#include "LogIPCPipeUnix.h"
#include "LogInstance.h"
#include "LogInternal.h"
#include "LogIPCStreamBase.h"
#include "LogSink.h"


namespace gnilk {
// We keep 100 log events in the cache
#ifndef GNILK_LOG_CACHE_CAPACITY
    #define GNILK_LOG_CACHE_CAPACITY 100
#endif

    class LogManager {
        friend LogSink;
    public:
        // Returns
        //   'true' continue iteration
        //   'false' stop iteration
        using SinkDelegate = std::function<bool(LogSink *)>;

        // Returns
        //   'true' continue iteration
        //   'false' stop iteration
        using LogDelegate = std::function<bool(const Log::Ref &)>;
    public:
        virtual ~LogManager();
        static LogManager &Instance();

    public:
        void Reset();
        void Initialize();
        Log::Ref GetOrAddLog(const std::string &name);
        Log::Ref GetExistingLog(const std::string &name);
        void IterateLogs(const LogDelegate &);

        // We don't handle sink's the same way ('GetExisting') as we can't gurantee they go out of scope
        void RegisterDefaultSinks();
        void AddSink(LogSink::Ref sink, const std::string &name);
        void AddSink(LogSink *sink, const std::string &name);
        bool RemoveSink(const std::string &name);
        void IterateSinks(const SinkDelegate &);

        LogIPCStreamBase &GetLogEventPipe() {
            return ipcHandler;
        }

        LogIPCBase &GetIPC() {
            return ipcHandler;
        }

        void SendToSinks();
    protected:
        void IterateCache(const LogCache::CachedEventDelgate &);

    private:
        LogManager() = default;
        void SendCacheToSink_NoLock(LogSink *sink);

    protected:
        bool isInitialized = false;
        size_t cacheCapacity = GNILK_LOG_CACHE_CAPACITY;

//        LogEventPipeUnix eventPipe;
        LogIPCFifoUnix ipcHandler;

        LogCache::Ref cache = {};
        std::mutex instLock;
        std::mutex sinkLock;
        std::unordered_map<std::string, LogInstance::Ref> logInstances;
        std::vector<LogSinkInstance::Ref> sinks;

        int eventMsgWritePipe = -1;
        int eventMsgReadPipe = -1;

    };

}

#endif //GNKLOG_LOGMANAGER_H
