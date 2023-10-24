//
// Created by gnilk on 23.10.23.
//

#include "LogSink.h"
#include "LogManager.h"

using namespace gnilk;

//
// Default implementation is to iterate the complete cache and empty it..
//
void LogSink::OnAttached() {
    LogManager::Instance().IterateCache([this](const LogEvent &event) -> bool {
        if (WithinRange(event.Level())) {
            if (Write(event) < 0) {
                return false;
            }
        }
        return true;
    });
}

