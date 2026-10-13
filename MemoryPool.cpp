#include "MemoryPool.h"
#include <vector>

MemoryPool::MemoryPool(size_t blockSize, size_t blockCount) {

    m_blockSize = blockSize;
    m_blockCount = blockCount;
    m_allocatedFlags = std::vector<bool>(blockCount, false);

    if (m_blockSize == 0 || m_blockCount == 0) {
        m_poolBuffer = nullptr;
        return;
    }

    m_poolBuffer = new char[m_blockSize * m_blockCount];

    for (size_t i = 0; i < m_blockCount; i++) {
        void* blockPtr = static_cast<void*>(m_poolBuffer + (i * m_blockSize));
        m_freeStack.push(blockPtr);
    }
}

MemoryPool::~MemoryPool() {
    delete[] m_poolBuffer;
}

void* MemoryPool::allocate() {
    if (m_freeStack.empty()) {
        return nullptr;
    }
    
    void* ptr = m_freeStack.pop();
    size_t index = getValidBlockIndex(ptr);
    m_allocatedFlags[index] = true;

    return ptr;
}

bool MemoryPool::deallocate(void* ptr) {
    size_t index = getValidBlockIndex(ptr);

    if (index == static_cast<size_t>(-1) || !m_allocatedFlags[index]) {
        return false;
    }

    m_allocatedFlags[index] = false;
    m_freeStack.push(ptr);
    return true;
}

size_t MemoryPool::availableBlocks() const {
    return m_freeStack.size();
}

size_t MemoryPool::allocatedBlocks() const {
    return m_blockCount - m_freeStack.size();
}

size_t MemoryPool::blockSize() const {
    return m_blockSize;
}

size_t MemoryPool::capacity() const {
    return m_blockCount * m_blockSize;
}



size_t MemoryPool::getValidBlockIndex(void* ptr) const {
    if (!ptr || !m_poolBuffer) return -1;

    char* p = static_cast<char*>(ptr);
    ptrdiff_t offset = p - m_poolBuffer;
    size_t totalBytes = m_blockSize * m_blockCount;

    if (offset < 0 || static_cast<size_t>(offset) >= totalBytes || (offset % m_blockSize != 0)) {
        return -1;
    }

    return static_cast<size_t>(offset / m_blockSize);
}