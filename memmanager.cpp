#include "memmanager.h"
#include "complexpart.h"

using namespace std;

MemoryManager externMemManager;

#define POOLSIZE 32

void* MemoryManager::allocate(size_t size)
{
    if (0 == emptyStorageHead)
        expandPoolSize();

    EmptyStorage* head = emptyStorageHead;
    emptyStorageHead = head->next;
    return head;
}

void MemoryManager::free(void* deleted)
{
    EmptyStorage* head = static_cast <EmptyStorage*> (deleted);
    head->next = emptyStorageHead;
    emptyStorageHead = head;
}


void MemoryManager::expandPoolSize()
{
    size_t size = (sizeof(ComplexObject) > sizeof(EmptyStorage*)) ? sizeof(ComplexObject) : sizeof(EmptyStorage*);
    EmptyStorage* head = reinterpret_cast<EmptyStorage*>(new char[size]);
    emptyStorageHead = head;

    for (int i = 0; i < POOLSIZE; i++) {
        head->next = reinterpret_cast<EmptyStorage*>(new char[size]);
        head = head->next;
    }

    head->next = 0;
}

void MemoryManager::cleanUp()
{
    EmptyStorage* nextPtr = emptyStorageHead;
    for (; nextPtr; nextPtr = emptyStorageHead) {
        emptyStorageHead = emptyStorageHead->next;
        delete[] nextPtr; // to avoid undefined behaviour
    }
}

