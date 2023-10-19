//
// Created by gnilk on 19.10.23.
//

#include <string>
#include <filesystem>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <poll.h>
#include "LogEventFifoUnix.h"

using namespace gnilk;

static std::string fifoBaseName= "/tmp/myfifo";

bool LogEventFifoUnix::Open() {
    if (isOpen) {
        return true;
    }

    // Create name with pid - we need to be able to run multiple app's in parallel...
    auto pid = getpid();
    fifoname = fifoBaseName + "_" + std::to_string(pid);

    // if it exists - remove it (I don't know...)
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

    // Also open it as a file-stream, we need to supply it to vfprintf in the writer, that's the whole purpose
    fifoStream = fopen(fifoname.c_str(), "rw+");
    if (fifoStream == nullptr) {
        close(fifofd);
        perror("fopen_write");
        return false;
    }

    // All good - tag and bag em...
    isOpen = true;
    return true;
}

void LogEventFifoUnix::Close() {
    if (!isOpen) {
        return;
    }

    fclose(fifoStream);
    close(fifofd);

    // remove the remaining fifo file
    if (std::filesystem::exists(fifoname)) {
        std::filesystem::remove(fifoname);
    }

    isOpen = false;
}

int32_t LogEventFifoUnix::Write(const void *data, size_t szBytes) {
    if (!isOpen) {
        return -1;
    }

    auto res = fwrite(data, szBytes, 1, fifoStream);
    if (res == 0) {
        perror("LogEventFifoUnix::Write");
        return -1;
    }
    fflush(fifoStream);

    return (int32_t)res;
}

int32_t LogEventFifoUnix::Read(void *dstBuffer, size_t maxBytes) {
    if (!isOpen) {
        return -1;
    }
    struct pollfd pfd {
            .fd = rwfd,
            .events = POLLIN,
            .revents = {},
    };

    if (poll(&pfd, 1, 0)==1) {
        auto res = read(rwfd, dstBuffer, maxBytes);
        if (res < 0) {
            perror("LogEventFifoUnix::Read");
        }
        return res;
    }
    return 0;
}