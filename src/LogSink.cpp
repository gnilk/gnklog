//
// Created by gnilk on 23.10.23.
//

#include "LogSink.h"
#include "LogManager.h"

using namespace gnilk;

void LogSink::OnAttached() {
    LogManager::Instance().IterateCache([this](const LogEvent &event) -> void {
        if (!WithinRange(event.Level())) return;
        Write(event);
    });
}

