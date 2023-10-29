//
// Created by gnilk on 19.10.23.
//

#ifndef GNKLOG_LOGEVENTFIFOUNIX_H
#define GNKLOG_LOGEVENTFIFOUNIX_H

#include "LogInternal.h"
namespace gnilk {
    class LogEventFifoUnix : public LogIPCBase {
    public:
        LogEventFifoUnix() = default;
        virtual ~LogEventFifoUnix() = default;

        bool Open() override;
        void Close() override;
        int32_t Write(const void *data, size_t szBytes) override;
        int32_t Read(void *dstBuffer, size_t maxBytes) override;

    private:
        bool isOpen = false;

        std::string fifoname = {};

        int fifofd = -1;
        int rwfd = -1;

    };
}


#endif //GNKLOG_LOGEVENTFIFOUNIX_H
