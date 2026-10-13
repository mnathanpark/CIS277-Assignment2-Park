#include <vector>
#include "Stack.h"

class MemoryPool {
    public:
        MemoryPool(size_t blockSize, size_t blockCount);
        ~MemoryPool();

        void* allocate();
        bool deallocate(void* ptr);

        size_t availableBlocks() const;
        size_t allocatedBlocks() const;
        size_t blockSize() const;
        size_t capacity() const;
    private:
        size_t m_blockSize;
        size_t m_blockCount;
        char* m_poolBuffer;

        Stack<void*> m_freeStack;
        std::vector<bool> m_allocatedFlags;

        size_t getValidBlockIndex(void* ptr) const;
};