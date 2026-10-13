#include "PacketQueue.h"
#include <cstring>

PacketQueue::PacketQueue(std::size_t blockSize, std::size_t blockCount)
    : m_pool(blockSize, blockCount),
      m_queue(blockCount),
      m_blockSize(blockSize)
{
}

bool PacketQueue::receive(std::size_t id, const unsigned char* bytes, std::size_t length)
{
    if (bytes == nullptr || length == 0 || length > m_blockSize)
    {
        return false;
    }

    if (m_queue.full() || m_pool.availableBlocks() == 0)
    {
        return false;
    }

    void* block = m_pool.allocate();
    if (block == nullptr)
    {
        return false;
    }

    std::memcpy(block, bytes, length);

    PacketDescriptor desc{id, block, length};
    if (!m_queue.enqueue(desc))
    {
        m_pool.deallocate(block);
        return false;
    }

    return true;
}

bool PacketQueue::processNext(std::size_t& id, std::vector<unsigned char>& bytes)
{
    PacketDescriptor desc;
    if (!m_queue.dequeue(desc))
    {
        return false;
    }

    bytes.resize(desc.length);
    if (desc.length > 0)
    {
        std::memcpy(bytes.data(), desc.blockPtr, desc.length);
    }
    id = desc.id;

    m_pool.deallocate(desc.blockPtr);
    return true;
}

std::size_t PacketQueue::pending() const
{
    return m_queue.size();
}

std::size_t PacketQueue::availableBlocks() const
{
    return m_pool.availableBlocks();
}