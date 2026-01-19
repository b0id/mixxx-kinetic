#pragma once

#include <rigtorp/SPSCQueue.h>

#include <cstddef>
#include <vector>

namespace mixxx {

/**
 * @class StreamRingBuffer
 * @brief A lock-free Single-Producer/Single-Consumer (SPSC) ring buffer
 * for streaming audio data from the FUSE reader thread to the engine thread.
 *
 * This class wraps rigtorp::SPSCQueue to provide a convenient interface
 * for writing/reading chunks of type T (e.g. float or int16_t).
 */
template<typename T>
class StreamRingBuffer {
  public:
    explicit StreamRingBuffer(size_t capacity)
            : m_queue(capacity) {
    }

    /**
     * @brief Writes data to the ring buffer.
     * @param data Pointer to the data to write.
     * @param count Number of elements to write.
     * @return Number of elements actually written.
     */
    size_t write(const T* data, size_t count) {
        size_t written = 0;
        for (size_t i = 0; i < count; ++i) {
            if (m_queue.try_push(data[i])) {
                written++;
            } else {
                break; // Buffer full
            }
        }
        return written;
    }

    /**
     * @brief Reads data from the ring buffer.
     * @param data Pointer to the buffer to read into.
     * @param count Number of elements to read.
     * @return Number of elements actually read.
     */
    size_t read(T* data, size_t count) {
        size_t readCount = 0;
        for (size_t i = 0; i < count; ++i) {
            if (m_queue.front()) {
                data[i] = *m_queue.front();
                m_queue.pop();
                readCount++;
            } else {
                break; // Buffer empty
            }
        }
        return readCount;
    }

    /**
     * @brief Checks if the buffer is empty.
     */
    bool empty() const {
        return m_queue.front() == nullptr;
    }

    /**
     * @brief Returns the capacity of the buffer.
     */
    size_t capacity() const {
        return m_queue.capacity();
    }

  private:
    rigtorp::SPSCQueue<T> m_queue;
};

} // namespace mixxx
