//
// Created by gnilk on 31.10.23.
//
//
// Created by gnilk on 30.10.23.
//
#include <string>
#include <vector>
#include <testinterface.h>
#include "logger.h"
#include "LogManager.h"
#include "LogIPCQueue.h"
#include "LogIPCPipeUnix.h"
#include "LogIPCFifoUnix.h"

using namespace gnilk;
extern "C" {
DLL_EXPORT int test_threads(ITesting *t);
DLL_EXPORT int test_threads_many(ITesting *t);
}

using namespace std::chrono_literals;

DLL_EXPORT int test_threads(ITesting *t) {
    return kTR_Pass;
}

// this will start 4 * number of HW threads available - the pass/fail is inspection if the output is well formed and properly interleaved
// We don't want to see broken strings...
DLL_EXPORT int test_threads_many(ITesting *t) {

    LogManager::Instance().Initialize();

    std::mutex lock;
    std::condition_variable cv;

    bool bStart = false;

    using DurationMS = std::chrono::duration<uint64_t, std::ratio<1, 1000> >;

    auto threadfun =[&lock, &cv, &bStart](int tc){
        printf("t%d started - waiting\n", tc);
        auto log = Logger::GetLog("thread");

        // Unless we unlock here we keep exclusivity
        std::unique_lock<std::mutex> lk(lock);
        cv.wait(lk);
        lk.unlock();

        printf("t%d Released\n",tc);
        for(int i=0;i<10;i++) {
            //log->Dbg("t{} counter={}", tc,i);
            log->Debug("t%d counter=%d this is a very long string with much thinking behind it lorem ipsum data scantables, if goats had wings they could fly but they would die as they were not able to eat grass!",tc,i);
            std::this_thread::yield();
        }
    };



    auto nThradsToStart = std::thread::hardware_concurrency();
    fmt::println("Should start: {}", nThradsToStart);

    // Start 4x the number of HW threads
    std::vector<std::thread> threads;
    for(int tc=0;tc<nThradsToStart*4;tc++) {
        threads.push_back(std::thread(threadfun,tc));
    }

        // Make sure we are all started...
    std::this_thread::sleep_for(100ms);

    // Kick em off...
    {
        std::lock_guard lk(lock);
        printf("starting\n");
    }

    cv.notify_all();

    printf("Joining\n");

    for(auto &t : threads) {
        t.join();
    }

    printf("Done\n");


    return kTR_Pass;
}
