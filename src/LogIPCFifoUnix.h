//
// Created by gnilk on 19.10.23.
//

#ifndef GNKLOG_LOGIPCFIFOUNIX_H
#define GNKLOG_LOGIPCFIFOUNIX_H

#include "LogInternal.h"
#include "LogIPCStreamBase.h"
namespace gnilk {
    class LogIPCFifoUnix : public LogIPCStreamBase {
    public:
        LogIPCFifoUnix() = default;
        virtual ~LogIPCFifoUnix() = default;

        // FIXME: Implement available!

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


#endif //GNKLOG_LOGIPCFIFOUNIX_H
