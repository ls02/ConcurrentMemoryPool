#ifndef __THREAD_CACHE_H__
#define __THREAD_CACHE_H__ 

#include "Common.hpp"

class ThreadCache 
{
    public:
        //申请和释放内存对象
        void* Allocate(size_t size);

        void Deallocate(void* ptr, size_t size);
    private:
        FreeList _freeList[];
};

#endif
