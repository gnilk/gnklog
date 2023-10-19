//
// Created by gnilk on 19.10.23.
//

#ifndef GNKLOG_LOGINTERNAL_H
#define GNKLOG_LOGINTERNAL_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

namespace gnilk {
    //
    // Various classes for internal use...
    //
    class LogIPCBase {
    public:
        virtual bool Open() {
            return false;
        }
        virtual void Close() {}
        virtual int32_t Write(const void *data, size_t szBytes) { return -1; }
        virtual int32_t Read(void *dstBuffer, size_t maxBytes) { return -1; }

        virtual FILE *GetWriteStream() { return nullptr; }
        virtual FILE *GetReadStream() { return nullptr; }
    };
}

#endif //GNKLOG_LOGINTERNAL_H
