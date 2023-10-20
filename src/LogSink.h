//
// Created by gnilk on 19.10.23.
//

#ifndef GNKLOG_LOGSINK_H
#define GNKLOG_LOGSINK_H

#include <string>
#include <vector>
#include <memory>

#include "LogEvent.h"



namespace gnilk {
    class ILogOutputSink {
    public:
        using Ref = std::shared_ptr<ILogOutputSink>;
    public:
        ILogOutputSink() = default;
        explicit ILogOutputSink(const std::string &sinkName) : name(sinkName) {}
        virtual ~ILogOutputSink() = default;

        const std::string &GetName() {
            return name;
        };

        virtual void Initialize([[maybe_unused]] const std::vector<const std::string &> &args) {

        }
        virtual bool Open() {
            return false;
        };
        virtual int Write(const LogEvent &logEvent) {
            return -1;
        };
        virtual void Flush() {  };
        virtual void Close() {  };

    private:
        const std::string name;
    };
    // legacy compatibility
    using LogBaseSink = ILogOutputSink;

}

#endif //GNKLOG_LOGSINK_H
