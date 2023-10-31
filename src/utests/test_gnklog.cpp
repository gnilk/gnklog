//
// Created by gnilk on 19.10.23.
// These unit-tests are meant to give me a base-line for when creating the new logger...
// I want the new logger to be a 90% drop-in-replacement...
//
// Note: This modules is intended as the integration testing point - it is the highest level. For now it is just
//       bogus - will be replaced later...
//
#include <string>
#include <vector>
#include <testinterface.h>

#include "gnklog.h"
#include "LogManager.h"

using namespace gnilk;
extern "C" {
DLL_EXPORT int test_gnklog(ITesting *t);
DLL_EXPORT int test_gnklog_exit(ITesting *t);
DLL_EXPORT int test_gnklog_debug(ITesting *t);
DLL_EXPORT int test_gnklog_readme(ITesting *t);
}

DLL_EXPORT int test_gnklog(ITesting *t) {
    return kTR_Pass;
}

DLL_EXPORT int test_gnklog_exit(ITesting *t) {
    auto logger = Logger::GetLogger("test2");

    logger->Debug("2 mamma: %d, %s", 4, "world");
    logger->Debug("2 Some more fascinating strings");
    logger->Debug("2 Data is debug");
    logger->SetLogLevelThreshold(kInfo);    // everything info and above is visible..

    logger->Debug("3 Data is debug");       // should not be visible
    logger->Info("3 info message");
    logger->Warning("3 warn message");

    Logger::Consume();

    return kTR_Pass;
}

DLL_EXPORT int test_gnklog_debug(ITesting *t) {

    auto logger = Logger::GetLogger("test");

    logger->Debug("mamma: %d, %s", 4, "world");
    logger->Debug("Some more fascinating strings");
    logger->Debug("Data is debug");

    LogManager::Instance().Reset();

    return kTR_Pass;
}
DLL_EXPORT int test_gnklog_readme(ITesting *t) {
    Logger::Initialize();
    auto logger = Logger::GetLogger("test");
    logger->Debug("This is some debug output with printf syntax: %d, %s", 4, "world");
    logger->Dbg("This is with fmt {}", 4);
    LogManager::Instance().Consume();

    return kTR_Pass;
}
