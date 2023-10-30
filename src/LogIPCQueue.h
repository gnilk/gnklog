//
// Created by gnilk on 30.10.23.
//

#ifndef GNKLOG_LOGIPCQUEUE_H
#define GNKLOG_LOGIPCQUEUE_H

#include "LogInternal.h"
#include "LogSafeQueue.h"

namespace gnilk {
    class LogIPCQueue : public LogIPCBase {
    protected:
        struct QueueMessage {
            LogEvent event;
            std::string dbgMessage;
        };
    public:
        int32_t WriteEvent(const LogEvent &event, const std::string &dbgMessage) override;
        int32_t ReadEvent(LogEvent &outEvent) override;
    private:
        SafeQueue<QueueMessage> queue;
    };
}


#endif //GNKLOG_LOGIPCQUEUE_H
