//
// Created by gnilk on 19.10.23.
//

//
// Event serialization using plain pipes
//

#include <cstdio>
#include <unistd.h>
#include <poll.h>

#include "LogIPCPipeUnix.h"

using namespace gnilk;

LogIPCPipeUnix::LogIPCPipeUnix() {

}

LogIPCPipeUnix::~LogIPCPipeUnix() {
    Close();
}

bool LogIPCPipeUnix::Open() {
    if (isOpen) {
        return true;
    }

    // Create a regular pipe - we use 'poll' to check if it has data
    int res = pipe(pipefd);

    if (res < 0) {
        perror("pipe2");
        return false;
    }
    isOpen = true;
    // Associate file-descriptors and create streams
    readfd = pipefd[0];
    writefd = pipefd[1];
    return true;
}

void LogIPCPipeUnix::Close() {
    if (!isOpen) {
        return;
    }
    close(readfd);
    close(writefd);
    isOpen = false;
}

bool LogIPCPipeUnix::Available() {
    struct pollfd pfd {
            .fd = readfd,
            .events = POLLIN,
            .revents = {},
    };

    // Non-blocking poll
    if (poll(&pfd, 1, 0)==1) {
        return true;
    }
    return false;
}

int32_t LogIPCPipeUnix::Write(const void *data, size_t szBytes) {
    if (!isOpen) {
        // Open here???
        return -1;
    }
    auto res =  (int32_t)write(writefd, data, szBytes);
    if (res < 0) {
        perror("LogEventPipeUnix::Write");
    }
    fsync(writefd);
    return res;

}

int32_t LogIPCPipeUnix::Read(void *dstBuffer, size_t maxBytes) {
    if (!isOpen) {
        return -1;
    }
    if (!Available()) {
        return 0;
    }
    auto res = (int32_t)read(readfd, dstBuffer, maxBytes);
    if (res < 0) {
        perror("LogEventPipeUnix::Read");
    }

    return res;
}
