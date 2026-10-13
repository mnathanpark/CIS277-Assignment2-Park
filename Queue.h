#pragma once

#include <vector>

template <typename T>
class Queue
{
public:
    explicit Queue(std::size_t capacity)
        : m_capacity(capacity > 0 ? capacity : 1),
          m_data(m_capacity),
          m_front(0),
          m_count(0)
    {
    }

    bool enqueue(const T& value)
    {
        if (full())
        {
            return false;
        }
        std::size_t tail = (m_front + m_count) % m_capacity;
        m_data[tail] = value;
        m_count++;
        return true;
    }

    bool dequeue(T& value)
    {
        if (empty())
        {
            return false;
        }
        value = m_data[m_front];
        m_front = (m_front + 1) % m_capacity;
        --m_count;
        return true;
    }

    bool front(T& value) const
    {
        if (empty())
        {
            return false;
        }
        value = m_data[m_front];
        return true;
    }

    std::size_t tail() const
    {
        return (m_front + m_count) % m_capacity;
    }

    bool empty() const
    {
        return m_count == 0;
    }

    bool full() const
    {
        return m_count == m_capacity;
    }

    std::size_t size() const
    {
        return m_count;
    }

    std::size_t capacity() const
    {
        return m_capacity;
    }

private:
    std::size_t m_capacity;
    std::vector<T> m_data;
    std::size_t m_front;
    std::size_t m_count;
};