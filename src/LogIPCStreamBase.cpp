//
// Created by gnilk on 30.10.23.
//

#include "LogIPCStreamBase.h"
#include <string.h>

using namespace gnilk;

int32_t LogIPCStreamBase::WriteEvent(const LogEvent &event, const std::string &dbgMessage) {
    // This the amount we need for the complete message, including string and terminating zero
    size_t nBytesTotal = sizeof(EventStreamMessage) + dbgMessage.length() + 1;

    // This is the buffer holding the serialized event + message
    auto ptrBuffer = alloca(nBytesTotal);
    if (ptrBuffer == nullptr) {
        return 0;
    }
    // Reset the buffer
    memset(ptrBuffer, 0, nBytesTotal);
    auto msg = static_cast<EventStreamMessage *>(ptrBuffer);

    // Copy over everything to a message - for serialization
    msg->version = 0x01;
    msg->timeStamp = event.timeStamp;
    msg->idSenderThread = event.idSenderThread;
    msg->idSenderProc = event.idSenderProc;
    msg->level = event.level;
    msg->messageLength = dbgMessage.size();
    strncpy(msg->sender, event.sender.c_str(), LOG_MAX_NAME_LEN);

    auto ptrString = ptrOffset<char *>(ptrBuffer, sizeof(EventStreamMessage));
    strncpy(ptrString, dbgMessage.c_str(), dbgMessage.size());

    // Write it over the event pipe..
    auto res = Write(ptrBuffer, nBytesTotal);

    if (res<0) {
        perror("LogIPCStremBase::WriteEvent");
        return res;
    }
    return res;

}
int32_t LogIPCStreamBase::ReadEvent(LogEvent &outEvent) {
    EventStreamMessage msg;

    auto res = Read(&msg, sizeof(msg));
    if (res < 0) {
        perror("LogEvent::Read");
        return res;
    }

    outEvent.timeStamp = msg.timeStamp;
    outEvent.idSenderThread = msg.idSenderThread;
    outEvent.idSenderProc = msg.idSenderProc;
    outEvent.sender = std::string(msg.sender);
    outEvent.level = msg.level;

    auto msgBuffer = (char *)alloca(msg.messageLength+1);
    memset(msgBuffer, 0, msg.messageLength + 1);
    int res2 = Read(msgBuffer, msg.messageLength + 1);
    if (res2 < 0) {
        return -1;
    }
    res += res2;

    outEvent.msgString = std::string(msgBuffer);

    return res;
}
