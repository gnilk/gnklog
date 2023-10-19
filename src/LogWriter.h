//
// Created by gnilk on 19.10.23.
//

#ifndef GNKLOG_LOGWRITER_H
#define GNKLOG_LOGWRITER_H

#include "LogCore.h"
#include <stdarg.h>
#include "LogEvent.h"

namespace gnilk {
    class LogWriter : public ILogger {
    public:
        static ILogger::Ref Create(const std::string &logName);
        virtual ~LogWriter();

    public:
        void DebugRef(const char *format, ...) override;
        void Debug(const char *format, ...) override;

    protected:
        //void ComposeString(const std::string_view format, va_list values);
    protected:
        LogWriter(const std::string &logName);
        void Initialize();
        void GenerateEventData(LogEvent &outEvent, LogLevel level);
        FILE *GetWriteStream();

    private:
        std::string name;
    };
}

#endif //GNKLOG_LOGWRITER_H
