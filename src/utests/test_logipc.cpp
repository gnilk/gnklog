//
// Created by gnilk on 30.10.23.
//
#include <string>
#include <vector>
#include <testinterface.h>
#include "logger.h"
#include "LogManager.h"
#include "LogIPCQueue.h"
#include "LogIPCPipeUnix.h"
#include "LogIPCFifoUnix.h"

using namespace gnilk;
extern "C" {
DLL_EXPORT int test_logipc(ITesting *t);
DLL_EXPORT int test_logipc_qavail(ITesting *t);
DLL_EXPORT int test_logipc_qpushpop(ITesting *t);
DLL_EXPORT int test_logipc_pavail(ITesting *t);
DLL_EXPORT int test_logipc_ppushpop(ITesting *t);
DLL_EXPORT int test_logipc_favail(ITesting *t);
DLL_EXPORT int test_logipc_fpushpop(ITesting *t);
}

DLL_EXPORT int test_logipc(ITesting *t) {
    return kTR_Pass;
}

// Queue handling
DLL_EXPORT int test_logipc_qavail(ITesting *t) {
    LogIPCQueue queue;

    TR_ASSERT(t, !queue.Available());
    LogEvent event = {};
    queue.WriteEvent(event, "wef");
    TR_ASSERT(t, queue.Available());

    return kTR_Pass;
}

DLL_EXPORT int test_logipc_qpushpop(ITesting *t) {
    LogIPCQueue queue;

    TR_ASSERT(t, !queue.Available());
    LogEvent event;
    event.idSenderProc = (pid_t)1;
    event.idSenderThread = std::this_thread::get_id();
    event.level = LogLevel::kDebug;
    TR_ASSERT(t, queue.WriteEvent(event, "wef") > 0);
    TR_ASSERT(t, queue.Available());
    LogEvent outEvent;
    TR_ASSERT(t, queue.ReadEvent(outEvent) > 0);
    TR_ASSERT(t, outEvent.idSenderThread == event.idSenderThread);
    TR_ASSERT(t, outEvent.idSenderProc == event.idSenderProc);

    return kTR_Pass;
}

// PIPE

DLL_EXPORT int test_logipc_pavail(ITesting *t) {
    LogIPCPipeUnix pipe;

    // Must call open on pipe
    TR_ASSERT(t, pipe.Open());

    TR_ASSERT(t, !pipe.Available());
    LogEvent event = {};
    pipe.WriteEvent(event, "wef");
    TR_ASSERT(t, pipe.Available());

    return kTR_Pass;
}
DLL_EXPORT int test_logipc_ppushpop(ITesting *t) {
    LogIPCPipeUnix pipe;

    TR_ASSERT(t, pipe.Open());

    TR_ASSERT(t, !pipe.Available());
    LogEvent event;
    event.idSenderProc = (pid_t)1;
    event.idSenderThread = std::this_thread::get_id();
    event.level = LogLevel::kDebug;
    TR_ASSERT(t, pipe.WriteEvent(event, "wef") > 0);
    TR_ASSERT(t, pipe.Available());
    LogEvent outEvent;
    TR_ASSERT(t, pipe.ReadEvent(outEvent) > 0);
    TR_ASSERT(t, outEvent.idSenderThread == event.idSenderThread);
    TR_ASSERT(t, outEvent.idSenderProc == event.idSenderProc);

    return kTR_Pass;
}

// FIFO
DLL_EXPORT int test_logipc_favail(ITesting *t) {
    LogIPCFifoUnix pipe;

    // Must call open on pipe
    TR_ASSERT(t, pipe.Open());

    TR_ASSERT(t, !pipe.Available());
    LogEvent event = {};
    pipe.WriteEvent(event, "wef");
    TR_ASSERT(t, pipe.Available());

    return kTR_Pass;
}
DLL_EXPORT int test_logipc_fpushpop(ITesting *t) {
    LogIPCFifoUnix pipe;

    TR_ASSERT(t, pipe.Open());

    TR_ASSERT(t, !pipe.Available());
    LogEvent event;
    event.idSenderProc = (pid_t)1;
    event.idSenderThread = std::this_thread::get_id();
    event.level = LogLevel::kDebug;
    TR_ASSERT(t, pipe.WriteEvent(event, "wef") > 0);
    TR_ASSERT(t, pipe.Available());
    LogEvent outEvent;
    TR_ASSERT(t, pipe.ReadEvent(outEvent) > 0);
    TR_ASSERT(t, outEvent.idSenderThread == event.idSenderThread);
    TR_ASSERT(t, outEvent.idSenderProc == event.idSenderProc);

    return kTR_Pass;
}
