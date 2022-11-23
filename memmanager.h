#pragma once

#include <sys/types.h> 

class ComplexObject;
class BaseMemoryManager
{
public:
    virtual void* allocate(size_t) = 0;
    virtual void  free(void*) = 0;
};

class MemoryManager : public BaseMemoryManager
{
    struct EmptyStorage
    {
        EmptyStorage* next;
    };
    void expandPoolSize();
    void cleanUp();
    EmptyStorage* emptyStorageHead;
public:
    MemoryManager() {
        emptyStorageHead = 0;
        expandPoolSize();
    }
    virtual ~MemoryManager() {
        cleanUp();
    }
    virtual void* allocate(size_t);
    virtual void free(void*);
};