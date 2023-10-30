//
// Created by gnilk on 19.10.23.
//

#ifndef GNKLOG_LOGINTERNAL_H
#define GNKLOG_LOGINTERNAL_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "LogSink.h"
#include <memory>

namespace gnilk {

    class LogIPCBase {
    public:
        using Ref = std::shared_ptr<LogIPCBase>;
    public:
        LogIPCBase() = default;
        virtual ~LogIPCBase() = default;

        virtual bool Open() {
            return true;
        }
        virtual void Close() {}

        // Must be overridden otherwise the system will deadlock
        // return true if there is data to be read false if no data is available
        virtual bool Available() { return false; }

        // Returns
        //       ok > 0
        //      nok < 0
        virtual int32_t WriteEvent(const LogEvent &event, const std::string &dbgMessage) { return false; }
        virtual int32_t ReadEvent(LogEvent &outEvent) { return false; }
    };

    template<typename T>
    T ptrOffset(void *basePtr, int32_t offset) {
        static_assert(std::is_pointer<T>());
        return (T )(reinterpret_cast<unsigned char *>(basePtr) + offset);
    }


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
        virtual LogSink *GetSink() = 0;
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
        LogSink *GetSink() override {
                return sink.get();
        }
        static Ref Create(LogSink::Ref sink, const std::string &name) {
            auto inst = new LogSinkInstanceManaged(sink, name);
            return std::unique_ptr<LogSinkInstanceManaged>(inst);
        }
    protected:
        LogSinkInstanceManaged(LogSink::Ref sinkRef, const std::string &sinkName) :
            LogSinkInstance(sinkName),
            sink(sinkRef) {
        }
        LogSink::Ref sink = nullptr;
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
        LogSink *GetSink() override {
            return sink;
        }
        static Ref Create(LogSink *sink, const std::string &name) {
            auto inst = new LogSinkInstanceUnmanaged(sink, name);
            return std::unique_ptr<LogSinkInstanceUnmanaged>(inst);
        }
    protected:
        LogSinkInstanceUnmanaged(LogSink *sinkPtr, const std::string &sinkName) :
                LogSinkInstance(sinkName),
                sink(sinkPtr) {

        }
        LogSink *sink = nullptr;
    };
}

#endif //GNKLOG_LOGINTERNAL_H
