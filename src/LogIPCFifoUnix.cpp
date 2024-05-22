//
// Created by gnilk on 19.10.23.
//

//
// Event serialization using fifo's - not sure about the portability here - will have to test on macOS at least
//

#include <string>
#include <filesystem>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <poll.h>
#include "LogIPCFifoUnix.h"

using namespace gnilk;

static std::string fifoBaseName= "/tmp/gnklog";

//
// FIXME: bugs found while incorporating in testrunner
//  - Separate open and 'connectto'
//  - Close should close rwfd and ONLY remove/close the fifofd when the 'server' side is closed
//

bool LogIPCFifoUnix::Open() {
    if (isOpen) {
        return true;
    }

    // Create name with pid - we need to be able to run multiple app's in parallel...
    auto pid = getpid();
    fifoname = fifoBaseName + "_" + std::to_string(pid);

    // if it exists - remove it - as it ought to be a left over from an old process or someone restarted the logger
    if (std::filesystem::exists(fifoname)) {
        std::filesystem::remove(fifoname);
    }

    // Create the fifo
    fifofd = mkfifo(fifoname.c_str(), 0666);
    if (fifofd < 0) {
        perror("mkfifo");
        return false;
    }

    // Open it in Read/Write mode - note: this might not be supported everywhere
    rwfd = open(fifoname.c_str(), O_RDWR);
    if (rwfd < 0) {
        close(fifofd);
        perror("open");
        return false;
    }

    // All good - tag and bag em...
    isOpen = true;
    return true;
}

void LogIPCFifoUnix::Close() {
    if (!isOpen) {
        return;
    }

    close(fifofd);

    // remove the remaining fifo file
    if (std::filesystem::exists(fifoname)) {
        std::filesystem::remove(fifoname);
    }

    isOpen = false;
}

bool LogIPCFifoUnix::Available() {
    struct pollfd pfd {
            .fd = rwfd,
            .events = POLLIN,
            .revents = {},
    };

    // Non-blocking poll...
    if (poll(&pfd, 1, 0)==1) {
        return true;
    }
    return false;
}


int32_t LogIPCFifoUnix::Write(const void *data, size_t szBytes) {
    if (!isOpen) {
        // Auto open here???
        return -1;
    }

    auto res =  (int32_t)write(rwfd, data, szBytes);
    if (res < 0) {
        perror("LogEventFifoUnix::Write");
    }
    fsync(rwfd);
    return res;
}

int32_t LogIPCFifoUnix::Read(void *dstBuffer, size_t maxBytes) {
    if (!isOpen) {
        return -1;
    }
    if (!Available()) {
        return 0;
    }
    auto res = read(rwfd, dstBuffer, maxBytes);
    if (res < 0) {
        perror("LogEventFifoUnix::Read");
    }
    return res;
}