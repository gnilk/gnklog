//
// Created by gnilk on 30.10.23.
//

#ifndef GNKLOG_LOGIPCSTREAMBASE_H
#define GNKLOG_LOGIPCSTREAMBASE_H

#include "LogInternal.h"
#include "LogEvent.h"

namespace gnilk {
    //
    // Various classes for internal use...
    //
    class LogIPCStreamBase : public LogIPCBase {
    protected:
#pragma pack(push, 1)
        struct EventStreamMessage {
            uint8_t version = 0;
            LogTimeStamp  timeStamp = {};
            pid_t   idSenderProc = {};
            std::thread::id idSenderThread = {};
            LogLevel level = kDebug;
            char sender[LOG_MAX_NAME_LEN] = {};
            uint16_t messageLength = 0;
            // Message follows - the buffer is allocated including the message length
        };
#pragma pack(pop)

    public:

        virtual bool Open() {
            return false;
        }
        virtual void Close() {}

        int32_t WriteEvent(const LogEvent &event, const std::string &dbgMessage) override;
        int32_t ReadEvent(LogEvent &outEvent) override;

        virtual int32_t Write(const void *data, size_t szBytes) { return -1; }
        virtual int32_t Read(void *dstBuffer, size_t maxBytes) { return -1; }
    };

}

#endif //GNKLOG_LOGIPCSTREAMBASE_H
