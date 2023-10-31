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
DLL_EXPORT int test_logipc(ITesting *t);
}

DLL_EXPORT int test_logipc(ITesting *t) {
    LogManager::Instance().Close();
    return kTR_Pass;
}
