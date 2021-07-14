#pragma once

#include "Common.h"
#include "ThreadCache.h"
#include "PageCache.h"

static inline void* ConcurrentAlloc(size_t size)
{
    if(size > MAX_BYTES)
    {
        Span* span = PageCache::GetInstence()->AllocBigPageObj(size);
        void* ptr = (void*)(span->_pageid << PAGE_SHIFT);
        return ptr;
    }
    else
    {
        if(tlslist == nullptr)
        {
            tlslist = new ThreadCache;
        }
        return tlslist->Allocate(size);
    }
}

static inline void ConcurrentFree(void* ptr)
{
    Span* span = PageCache::GetInstence()->MapObjectToSpan(ptr);
    size_t size = span->_objsize;
    if(size > MAX_BYTES)
        PageCache::GetInstence()->FreeBigPageObj(ptr, span);
    else
        tlslist->Deallocate(ptr, size);
}