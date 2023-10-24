//
// Created by gnilk on 24.10.23.
//
#include <testinterface.h>
#include "gnklog.h"
#include "LogCache.h"
#include "LogManager.h"
#include "fmt/format.h"

using namespace gnilk;
extern "C" {
DLL_EXPORT int test_logcache(ITesting *t);
DLL_EXPORT int test_logcache_create(ITesting *t);
DLL_EXPORT int test_logcache_sinkattach(ITesting *t);
}

DLL_EXPORT int test_logcache(ITesting *t) {
    return kTR_Pass;
}

DLL_EXPORT int test_logcache_create(ITesting *t) {
    auto cache = LogCache::Create(10);

    for(size_t i=0;i<cache->Capacity();i++) {
        auto v = cache->Next();
        v->msgString = fmt::format("{}", i);
    }


    // Should now have filled up the cache
    TR_ASSERT(t, cache->Size() == cache->Capacity());
    TR_ASSERT(t, cache->AtAbsolute(0)->msgString == "0");
    TR_ASSERT(t, cache->AtAbsolute(1)->msgString == "1");

    printf("Size=%zu\n", cache->Size());
    cache->Iterate( [](const LogEvent &event) -> bool {
        printf("%s\n",event.msgString.c_str());
        return true;
    });
    auto v = cache->Next();
    v->msgString = fmt::format("{}", 11);
    v = cache->Next();
    v->msgString = fmt::format("{}", 12);

    // 11 and 12 should now be at the front - since we have wrapped the ring..
    TR_ASSERT(t, cache->AtAbsolute(0)->msgString == "11");
    TR_ASSERT(t, cache->AtAbsolute(1)->msgString == "12");

    TR_ASSERT(t, cache->Size() == 10);
    printf("Size=%zu\n", cache->Size());
    cache->Iterate( [](const LogEvent &event) -> bool {
        printf("%s\n",event.msgString.c_str());
        return true;
    });


    return kTR_Pass;
}

class VectorSink : public LogSink {
public:
    VectorSink() = default;
    virtual ~VectorSink() = default;

    int Write(const LogEvent &logEvent) override {
        logmessages.push_back(logEvent.msgString);
        return 1;
    };
public:
    std::vector<std::string> logmessages;
};

DLL_EXPORT int test_logcache_sinkattach(ITesting *t) {
    VectorSink mySink;
    LogManager::Instance().Reset();

    auto logger = Logger::GetLogger("attach");
    logger->Debug("1");
    logger->Debug("2");

    TR_ASSERT(t, mySink.logmessages.size() == 0);
    LogManager::Instance().AddSink(&mySink, "mysink");
    TR_ASSERT(t, mySink.logmessages.size() == 2);
    logger->Debug("3");
    TR_ASSERT(t, mySink.logmessages.size() == 3);

    return kTR_Pass;
}
