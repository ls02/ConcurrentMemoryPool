#include "ThreadCache.hpp"

void* ThreadCache::Allocate(size_t size)
{
    assert(size <= MAX_BYTES);

    size_t alignSize = SizeClass::RoundUp(size);
    size_t 
}

void ThreadCache::Deallocate(void* ptr, size_t size)
{

}
