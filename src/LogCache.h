//
// Created by gnilk on 24.10.23.
//

#ifndef GNKLOG_LOGCACHE_H
#define GNKLOG_LOGCACHE_H

#include <functional>
#include <memory>
#include "LogEvent.h"

namespace gnilk {

    class LogManager;
    // Consider implementing a c++ iterator
    class LogCache {
        friend LogManager;
    public:
        using Ref = std::shared_ptr<LogCache>;
        // The event delegate is used when iterating the cache
        // return values
        //  'true' to continue the iteration
        //  'false' to stop the iteration
        using CachedEventDelgate = std::function<bool(const LogEvent &)>;
    public:
        LogCache() = default;
        explicit LogCache(size_t useCapacity);
        virtual ~LogCache();

        static Ref Create(size_t useCapacity);

        void Clear();
        LogEvent *Next();
        size_t Size() const;
        __inline size_t Capacity() const {
            return capacity;
        }

        LogEvent *AtAbsolute(size_t idx);

        void Iterate(const CachedEventDelgate &);
    private:
        size_t idxHead = {};
        size_t idxTail = {};
        size_t capacity = 0;
        bool wpc = false;       // try to find a way to get rid of this...
        LogEvent *items = {};
    };
}


#endif //GNKLOG_LOGCACHE_H
