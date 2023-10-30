//
// Created by gnilk on 30.10.23.
//

#include "LogIPCQueue.h"

using namespace gnilk;

int32_t LogIPCQueue::WriteEvent(const LogEvent &event, const std::string &dbgMessage) {
    QueueMessage msg;
    msg.event = event;
    msg.dbgMessage = dbgMessage;
    queue.push(msg);
    return 1;
}
int32_t LogIPCQueue::ReadEvent(LogEvent &outEvent) {
    auto msg = queue.pop();
    outEvent = msg.event;
    outEvent.msgString = msg.dbgMessage;
    return 1;
}
