//
// Created by gnilk on 19.10.23.
//

//
// Created by gnilk on 19.10.23.
// These unit-tests are meant to give me a base-line for when creating the new logger...
// I want the new logger to be a 90% drop-in-replacement...
//
#include <string>
#include <vector>
#include <testinterface.h>

#include "gnklog.h"

using namespace gnilk;
extern "C" {
DLL_EXPORT int test_logger(ITesting *t);
DLL_EXPORT int test_logger_exit(ITesting *t);
DLL_EXPORT int test_logger_debug(ITesting *t);
DLL_EXPORT int test_logger_debugref(ITesting *t);
}

DLL_EXPORT int test_logger(ITesting *t) {
    return kTR_Pass;
}

DLL_EXPORT int test_logger_exit(ITesting *t) {
    auto logger = Logger::GetLogger("test2");

    logger->Debug("2 mamma: %d, %s", 4, "world");
    logger->Debug("2 Some more fascinating strings");
    logger->Debug("2 Data is debug");
    return kTR_Pass;
}

DLL_EXPORT int test_logger_debug(ITesting *t) {

    auto logger = Logger::GetLogger("test");

    logger->Debug("mamma: %d, %s", 4, "world");
    logger->Debug("Some more fascinating strings");
    logger->Debug("Data is debug");
    return kTR_Pass;
}
