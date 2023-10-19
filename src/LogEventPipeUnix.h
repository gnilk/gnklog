//
// Created by gnilk on 19.10.23.
//

#ifndef GNKLOG_LOGEVENTPIPEUNIX_H
#define GNKLOG_LOGEVENTPIPEUNIX_H

#include <memory>
#include "LogInternal.h"

namespace gnilk {

    class LogEventPipeUnix : public LogIPCBase {
    public:
        LogEventPipeUnix();
        virtual ~LogEventPipeUnix();
        bool Open() override;
        void Close() override;
        int32_t Write(const void *data, size_t szBytes) override;
        int32_t Read(void *dstBuffer, size_t maxBytes) override;

        FILE *GetWriteStream() override {
            return writeStream;
        }
        FILE *GetReadStream() override {
            return readStream;
        }

    private:
        bool isOpen = false;
        int pipefd[2] = {};
        int writefd = -1;
        int readfd = -1;

        FILE *writeStream = nullptr;
        FILE *readStream = nullptr;
    };

}

#endif //GNKLOG_LOGEVENTPIPEUNIX_H
