#ifndef __OBJECT_POOL_H__
#define __OBJECT_POOL_H__ 

#include <iostream>
#include <vector>
#include <ctime>

#ifdef _WIN32
#include <Windows.h>
#else

#endif

// 定长内存池
//template<size_t N>
//class ObjectPool 
//{
//
//};

inline static void* SystemAlloc(size_t kpage)
{
#ifdef _WIN32
    void* ptr = VirtualAlloc(0, kpage << 13,MEM_COMMIT | MEM_RESERVE,
            PAGE_READWRITE);
#else
    // linux下brk mmap等
#endif
    if (ptr == nullptr)
        throw std::bad_alloc();
    return ptr;
}

template<class T>
class ObjectPool 
{
    public:
        T* New()
        {
            T* obj = nullptr;
            // 优先吧还回来的内存块对象，再次重复利用
            if (nullptr != _freeList)
            {
                void* next = *((void**)_freeList);
                obj = (T*)_freeList;
                _freeList = next;

                return obj;
            }
            else
            {
                // 剩余的内存不够一个对象大小时，则重新开大块空间
                if (_remainBytes <  sizeof(T))
                {
                    _remainBytes = 128 * 1024;
                    _memory = (char*)SystemAlloc(_remainBytes >> 13);
                    if (nullptr == _memory)
                    {
                        throw std::bad_alloc();
                    }
                }

                obj = (T*)_memory;
                size_t objSize = sizeof(T) < sizeof(void*) ? sizeof(void*) : sizeof(T);
                _memory += objSize;
                _remainBytes -= objSize;
            }

            // 定位new，显示调用T的构造函数初始化
            new(obj)T;

            return obj;
        }

        void Delete(T* obj)
        {
            // 显式调用析构函数清理对象
            obj->~T();

            if (nullptr == _freeList)
            {
                _freeList = obj;
                *(void**)obj = nullptr;
            }
            else 
            {
                //头插
                *(void**)obj = _freeList;
                _freeList = obj;
            }
        }
    private:
        char* _memory = nullptr; // 指向大块内存的指针
        size_t _remainBytes = 0; //bin大块内在切分过程中剩余字节数
        void* _freeList = nullptr;  // 还回来的过程中链接的自由链表的头指针
};

struct TreeNode
{
    int _val;
    TreeNode* _left;
    TreeNode* _right;
    TreeNode()
        :_val(0)
         , _left(nullptr)
         , _right(nullptr)
    {}
};

void TestObjectPool()
{
    // 申请释放的轮次
    const size_t Rounds = 3;
    // 每轮申请释放多少次
    const size_t N = 100000;
    size_t begin1 = clock();
    std::vector<TreeNode*> v1;
    v1.reserve(N);
    for (size_t j = 0; j < Rounds; ++j)
    {
        for (int i = 0; i < N; ++i)
        {
            v1.push_back(new TreeNode);
        }
        for (int i = 0; i < N; ++i)
        {
            delete v1[i];
        }
        v1.clear();
    }
    size_t end1 = clock();
    ObjectPool<TreeNode> TNPool;
    size_t begin2 = clock();

    std::vector<TreeNode*> v2;
    v2.reserve(N);
    for (size_t j = 0; j < Rounds; ++j)
    {
        for (int i = 0; i < N; ++i)
        {
            v2.push_back(TNPool.New());
        }
        for (int i = 0; i < 100000; ++i)
        {
            TNPool.Delete(v2[i]);
        }
        v2.clear();
    }
    size_t end2 = clock();
    std::cout <<"new cost time:" <<end1 - begin1 << std::endl;
    std::cout <<"object pool cost time:" <<end2 - begin2 << std::endl;
}

#endif
