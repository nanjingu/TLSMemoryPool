#pragma once

#include "Common.h"

class PageCache
{
public:
    static PageCache* GetInstence()
    {
        return &_inst;
    }

    Span* AllocBigPageObj(size_t size);
    void FreeBigPageObj(void* ptr, Span* span);

    Span* _NewSpan(size_t size);
    Span* NewSpan(size_t size);

    Span* MapObjectToSpan(void* obj);
    void ReleaseSpanToPageCache(Span* span);
    
private:
    SpanList _spanlist[NPAGES];
    std::unordered_map<PageID, Span*> _idspanmap;
    std::mutex _mutex;

    PageCache(){}
    PageCache(const PageCache&) = delete;
    static PageCache _inst;
};