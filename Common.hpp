#ifndef __COMMON_H__
#define __COMMON_H__ 

#include <iostream>
#include <vector>
#include <time.h>
#include <assert.h>

static const size_t MAX_BYTES = 256 * 1024;


void*& NextObj(void* obj)
{
    return *(void**)obj;
}

// 管理切分好的小对象的自由链表
class FreeList
{
    public:
        void Push(void* obj)
        {
            assert(obj);

            //头插
            //*(void**)obj = _freeList;
            NextObj(obj) = _freeList;
            _freeList = obj;
        }

        void* Pop()
        {
            assert(_freeList);

            //头删
            void* obj = _freeList;
            _freeList = NextObj(obj);

            return obj;
        }
    private:
        void* _freeList;
};

//计算对象大小的对其映射规则
class SizeClass
{
    public:
        //size_t _RoundUp(size_t size, size_t alignNum)
        //{
        //    size_t alignSize = 0;
        //   if (size % 8 != 0)
        //   {
        //       alignSize = (size / alignNum + 1) * alignNum;
        //   }
        //   else
        //   {
        //       alignSize = size;
        //   }

        //   return alignSize;
        //}
        
        static inline size_t _RoundUp(size_t bytes, size_t alignNum)
        {
           return ((bytes + alignNum - 1) & ~(alignNum - 1));
        }

        static inline size_t RoundUp(size_t size)
        {
            if (size <= 128)
            {
                return _RoundUp(size, 8);
            }
            else if (size <= 1024)
            {

            }
            else if (size <= 8 * 1024)
            {

            }
            else if (size <= 64 * 1024)
            {

            }
            else if (size <= 256 * 1024)
            {

            }
            else
            {
                assert(false);

                return -1;
            }
        }
}

#endif
