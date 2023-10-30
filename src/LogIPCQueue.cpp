//
// Created by gnilk on 30.10.23.
//

#include "LogIPCQueue.h"

using namespace gnilk;

void LogIPCQueue::Close() {
    isClosed = true;
    queue.stop();
}

bool LogIPCQueue::Available() {
    return !queue.empty();
}

int32_t LogIPCQueue::WriteEvent(const LogEvent &event, const std::string &dbgMessage) {
    if (isClosed) {
        return -1;
    }

    QueueMessage msg;
    msg.event = event;
    msg.dbgMessage = dbgMessage;
    queue.push(msg);
    return 1;
}

int32_t LogIPCQueue::ReadEvent(LogEvent &outEvent) {
    if (isClosed) {
        return -1;
    }
    auto msg = queue.pop();
    // stopped??
    if (!msg.has_value()) {
        return -1;
    }
    outEvent = msg->event;
    outEvent.msgString = msg->dbgMessage;
    return 1;
}
