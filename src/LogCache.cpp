//
// Created by gnilk on 24.10.23.
//

#include "LogCache.h"

using namespace gnilk;
LogCache::LogCache(size_t useCapacity) : capacity(useCapacity) {
    items = new LogEvent[capacity];
}

LogCache::~LogCache() {
    if (items != nullptr) {
        delete[] items;
    }
}

LogCache::Ref LogCache::Create(size_t useCapacity) {
    return std::make_shared<LogCache>(useCapacity);
}


void LogCache::Clear() {
        wpc = false;
        idxHead = 0;
        idxTail = 0;
}

LogEvent *LogCache::Next() {
    // lock here

    // This write will overwrite IF we have wrapped..
    // so move the tail one up... this makes the read follow the tail
    if (wpc) {
        idxTail = (idxHead + 1) % capacity;
    }
    auto next = &items[idxHead];

    wpc = false;
    if ((idxHead = ((idxHead + 1) % capacity)) == idxTail) {
        wpc = true;
    }

    return next;
}


void LogCache::Iterate(const CachedEventDelgate &delegate) {
    // Lock here
    auto idxIterateTail = idxTail;

    auto num = Size();
    // fixme: replace with 'while(num) { num-- };'
    for(size_t i=0;i<num;i++) {
        if (!delegate(items[idxIterateTail])) {
            break;
        }
        idxIterateTail = (idxIterateTail + 1) % capacity;
    }
}

LogEvent *LogCache::AtAbsolute(size_t idx) {
    if (idx >= capacity) {
        return nullptr;
    }
    return &items[idx];
}

size_t LogCache::Size() const {
    return ((idxHead + (capacity+1) - 1 * wpc) - idxTail) % (capacity+1);
}

