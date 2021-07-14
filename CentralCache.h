#pragma once

#include "Common.h"

class CentralCache
{
public:
    static CentralCache* Getinstence()
    {
        return &_inst;
    }

    Span* GetOneSpan(SpanList& spanlist, size_t byte_size);
    size_t FetchRangeObj(void*& start, void*& end, size_t n, size_t byte_size);
    void ReleaseListToSpan(void* statrt, size_t size);

private:
    SpanList _spanlist[NLISTS];

    CentralCache(){}
    CentralCache(CentralCache&) = delete;
    static CentralCache _inst;
}