//
// Created by gnilk on 19.10.23.
//

#include <iostream>
#include <string>
#include <cstdio>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <filesystem>
#include <poll.h>

#include "LogEventPipeUnix.h"

using namespace gnilk;

LogEventPipeUnix::LogEventPipeUnix() {

}

LogEventPipeUnix::~LogEventPipeUnix() {
    Close();
}

bool LogEventPipeUnix::Open() {
    if (isOpen) {
        return true;
    }

    // Create a regular non-blocking pipe
    //int res = pipe2(pipefd, O_NONBLOCK);
    int res = pipe(pipefd);

    if (res < 0) {
        perror("pipe2");
        return false;
    }
    isOpen = true;
    // Associate file-descriptors and create streams
    readfd = pipefd[0];
    writefd = pipefd[1];
    readStream = fdopen(readfd, "r");
    writeStream = fdopen(writefd, "w");
    return true;
}

void LogEventPipeUnix::Close() {
    if (!isOpen) {
        return;
    }
    fclose(readStream);
    fclose(writeStream);
    close(readfd);
    close(writefd);
    isOpen = false;
}

int32_t LogEventPipeUnix::Write(const void *data, size_t szBytes) {
    if (!isOpen) {
        return -1;
    }
    auto res =  (int32_t)write(writefd, data, szBytes);
    if (res < 0) {
        perror("LogEventPipeUnix::Write");
    }
    return res;

}

int32_t LogEventPipeUnix::Read(void *dstBuffer, size_t maxBytes) {
    if (!isOpen) {
        return -1;
    }

    struct pollfd pfd {
        .fd = readfd,
        .events = POLLIN,
        .revents = {},
    };

    if (poll(&pfd, 1, 0)==1) {
        auto res = (int32_t)read(readfd, dstBuffer, maxBytes);
        if (res < 0) {
            perror("LogEventPipeUnix::Read");
        }
        return res;
    }

    return 0;
}
