//
// Created by gnilk on 19.10.23.
//

#ifndef GNKLOG_LOGINTERNAL_H
#define GNKLOG_LOGINTERNAL_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "LogSink.h"

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

    //
    // These classes are used to hold LogSink's in the LogManager class
    // I differentiate between a managed (shared_ptr) and unmanged (raw ptr).
    // Because I want to support the use case where you declare you sink's statically and just pass the instance pointer to them
    //
    class LogSinkInstance {
    public:
        using Ref = std::unique_ptr<LogSinkInstance>;
    public:
        virtual ~LogSinkInstance() = default;
        virtual ILogOutputSink *GetSink() = 0;
        const std::string &GetName() {
            return name;
        }
    protected:
        LogSinkInstance(const std::string &sinkName) : name(sinkName) {

        }
    protected:
        const std::string name;
    };

    class LogSinkInstanceManaged : public LogSinkInstance {
    public:
        virtual ~LogSinkInstanceManaged() = default;
        ILogOutputSink *GetSink() override {
                return sink.get();
        }
        static Ref Create(ILogOutputSink::Ref sink, const std::string &name) {
            auto inst = new LogSinkInstanceManaged(sink, name);
            return std::unique_ptr<LogSinkInstanceManaged>(inst);
        }
    protected:
        LogSinkInstanceManaged(ILogOutputSink::Ref sinkRef, const std::string &sinkName) :
            LogSinkInstance(sinkName),
            sink(sinkRef) {
        }
        ILogOutputSink::Ref sink = nullptr;
    };

    //
    // This handles unmanaged sinks, i.e. the actual sink ptr
    // This is for when (in the app-code) someone does:
    //  static MySpecialLogSink mySink;
    //
    //  Logger::AddSink(&mySink);
    //
    class LogSinkInstanceUnmanaged : public LogSinkInstance {
    public:
        virtual ~LogSinkInstanceUnmanaged() = default;
        ILogOutputSink *GetSink() override {
            return sink;
        }
        static Ref Create(ILogOutputSink *sink, const std::string &name) {
            auto inst = new LogSinkInstanceUnmanaged(sink, name);
            return std::unique_ptr<LogSinkInstanceUnmanaged>(inst);
        }
    protected:
        LogSinkInstanceUnmanaged(ILogOutputSink *sinkPtr, const std::string &sinkName) :
                LogSinkInstance(sinkName),
                sink(sinkPtr) {

        }
        ILogOutputSink *sink = nullptr;
    };
}

#endif //GNKLOG_LOGINTERNAL_H
