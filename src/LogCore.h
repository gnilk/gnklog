//
// Created by gnilk on 19.10.23.
//

#ifndef GNKLOG_LOGCORE_H
#define GNKLOG_LOGCORE_H

#include <chrono>

namespace gnilk {

    // The max name of a logger instance
#ifndef LOG_MAX_NAME_LEN
    #define LOG_MAX_NAME_LEN 64
#endif

#ifdef WIN32
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <Windows.h>
    // Will this pick up mine instead???
    #include <process.h>
    #ifdef _MSC_VER
        typedef DWORD pid_t;
    #endif
#endif
    using LogClock = std::chrono::system_clock;
    using LogTimeStamp = std::chrono::time_point<LogClock>;

    enum LogLevel {
        kNone     = 0,
        kDebug    = 100,
        kInfo     = 200,
        kWarning  = 300,
        kError    = 400,
        kCritical = 500,
        kCustom   = 600,
    };

    enum class IPCMechanism {
        kQueue,     // in-process queue, this is ok if you are not calling fork and so forth (default)
        kFifo,      // in case you have out-of-process stuff sharing logger (fork), you can use this or 'pipe'
        kPipe,      // in case you have out-of-process stuff sharing logger (fork), you can use this or 'fifo'
    };


}

#endif //GNKLOG_LOGCORE_H
