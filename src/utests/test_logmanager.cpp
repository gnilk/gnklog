//
// Created by gnilk on 30.10.23.
//
#include <string>
#include <vector>
#include <testinterface.h>
#include "logger.h"
#include "LogManager.h"

using namespace gnilk;
extern "C" {
DLL_EXPORT int test_logmanager(ITesting *t);
DLL_EXPORT int test_logmanager_reset(ITesting *t);
}

DLL_EXPORT int test_logmanager(ITesting *t) {
    // The test here - it should work and not crash!
    LogManager::Instance().Initialize();
    auto logA = Logger::GetLog("logA");
    logA->Debug("hello");
    LogManager::Instance().Close();
    TR_ASSERT(t, Log::kStatus::kSendError == logA->Debug("closed"));

    // Now - logA will be 'orphaned' - i.e. unknown to the log-manager...
    LogManager::Instance().Initialize();
    TR_ASSERT(t, Log::kStatus::kOk == logA->Debug("Reopened"));
    LogManager::Instance().Close();


    int logCounter = 0;
    LogManager::Instance().IterateLogs([&logCounter](const Log::Ref &log) ->bool {
        logCounter++;
        return true;
    });
    // Log's should still be present!!
    TR_ASSERT(t, logCounter == 1);

    // This should simply not crash..
    LogManager::Instance().Initialize();
    LogManager::Instance().Close();

    LogManager::Instance().Initialize();
    LogManager::Instance().Close();

    return kTR_Pass;
}

DLL_EXPORT int test_logmanager_reset(ITesting *t) {
    LogManager::Instance().Reset();
    return kTR_Pass;
}