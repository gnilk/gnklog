//
// Created by gnilk on 19.10.23.
//

#ifndef GNKLOG_LOGIPCPIPEUNIX_H
#define GNKLOG_LOGIPCPIPEUNIX_H

#include <memory>
#include "LogInternal.h"
#include "LogIPCStreamBase.h"

namespace gnilk {

    class LogIPCPipeUnix : public LogIPCStreamBase {
    public:
        LogIPCPipeUnix();
        virtual ~LogIPCPipeUnix();
        bool Open() override;
        void Close() override;
        int32_t Write(const void *data, size_t szBytes) override;
        int32_t Read(void *dstBuffer, size_t maxBytes) override;

    private:
        bool isOpen = false;
        int pipefd[2] = {};
        int writefd = -1;
        int readfd = -1;
    };

}

#endif //GNKLOG_LOGIPCPIPEUNIX_H
