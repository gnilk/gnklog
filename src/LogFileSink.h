//
// Created by gnilk on 25.10.23.
//

#ifndef GNKLOG_LOGFILESINK_H
#define GNKLOG_LOGFILESINK_H

#include <vector>
#include <string>
#include <stdio.h>
#include "LogSink.h"

namespace gnilk {
    class LogFileSink : public LogSink {
    public:
        LogFileSink() : LogSink("filesink") {}
        virtual ~LogFileSink();

        void Initialize(const std::vector<std::string> &args) override;


        static LogSink::Ref Create();
        int Write(const LogEvent &logEvent) override;

        void Flush() override;
        void Close() override;

    protected:
        void ParseArgs(const std::vector<std::string> &args);
        void OpenLogFile();
    private:
        bool bAppend = false;
        bool bFlushOnWrite = false;
        std::string filename = "logfile.log";
        FILE *fOut = nullptr;
    };
}


#endif //GNKLOG_LOGFILESINK_H
