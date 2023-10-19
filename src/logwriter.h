//
// Created by gnilk on 19.10.23.
//

#ifndef GNKLOG_LOGWRITER_H
#define GNKLOG_LOGWRITER_H

#include "logcore.h"

namespace gnilk {
    class LogWriter : public ILogger {
    public:
        static ILogger::Ref Create(const std::string &logName);
        virtual ~LogWriter() = default;

    public:
        void WriteLine(const std::string &format,...) override;
        void WriteLine(LogLevel logLevel, const std::string &format,...) override;

        void Critical(const std::string &format,...) override;
        void Error(const std::string &format, ...) override;
        void Warning(const std::string &format, ...) override;
        void Info(const std::string &format, ...) override;
        void Debug(const std::string &format, ...) override;

    protected:
        LogWriter(const std::string &logName);
    private:
        std::string name;
    };
}

#endif //GNKLOG_LOGWRITER_H
