#include "ThreadCache.h"
#include "CentralCache.h"

void* ThreadCache::FetchFromCentralCache(size_t index, size_t size)
{
    Freelist* freelist = &_freelist[index];
    size_t maxsize = freelist->MaxSize();
    size_t numtomove = min(SizeClass::NumMoveSize(size), maxsize);
    void* start = nullptr;
    void* end = nullptr;

    size_t batchsize = CentralCache::Getinstence()->FetchRangeObj(start, end, numtomove, size);
    if(batchsize > 1)
    {
        freelist->PushRange(NEXT_OBJ(start), end, batchsize - 1);
    }
    if(batchsize > freelist->MaxSize())//diff
    {
        freelist->SetMaxSize(batchsize);//diff
    }
    return start;
}

void ThreadCache::ListTooLong(Freelist* freelist, size_t size)
{
    void* start = freelist->PopRange();
    CentralCache::Getinstence()->ReleaseListToSpan(start, size);
}

void* ThreadCache::Allocate(size_t size)
{
    size_t index = SizeClass::Index(size);
    Freelist* freelist = &_freelist[index];
    if(!freelist->Empty())
    {
        return freelist->Pop();
    }
    else
    {
        return FetchFromCentralCache(index, SizeClass::Roundup(size));
    }
}

void ThreadCache::Deallocate(void* ptr, size_t size)
{
    size_t index = SizeClass::Index(size);
    Freelist* freelist = &_freelist[index];

    freelist->Push(ptr);
    if(freelist->Size() >= freelist->MaxSize())
    {
        ListTooLong(freelist, size);
    }
}