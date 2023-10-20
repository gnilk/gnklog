//
// Created by gnilk on 19.10.23.
//

#ifndef GNKLOG_LOGCONSOLESINK_H
#define GNKLOG_LOGCONSOLESINK_H

#include "LogCore.h"
#include "LogSink.h"

namespace gnilk {

    class LogConsoleSink : public ILogOutputSink {
    public:
        LogConsoleSink() : ILogOutputSink("console") {}
        virtual ~LogConsoleSink() = default;

        static ILogOutputSink::Ref Create();
        int Write(const LogEvent &logEvent) override;
    };
}


#endif //GNKLOG_LOGCONSOLESINK_H
