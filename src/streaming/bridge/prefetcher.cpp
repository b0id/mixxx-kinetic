#include "prefetcher.h"

#include "streaming/bridge/rangefetcher.h"
#include "streaming/bridge/sparsecache.h"

#include <QDebug>
#include <algorithm>
#include <iostream>

namespace mixxx {
namespace kinetic {

Prefetcher::Prefetcher(RangeFetcher* fetcher)
        : m_fetcher(fetcher),
          m_running(true) {
    // Start background worker thread
    m_worker = std::thread(&Prefetcher::workerThread, this);
}

Prefetcher::~Prefetcher() {
    // Signal worker to stop
    m_running = false;
    m_cv.notify_all();

    // Wait for worker to finish
    if (m_worker.joinable()) {
        m_worker.join();
    }
}

void Prefetcher::startPrefetch(InodeNumber ino,
        const QUrl& url,
        SparseCache* cache,
        int64_t totalSize) {
    std::lock_guard<std::mutex> lock(m_mutex);

    // Create new inode state
    m_states[ino] = std::make_unique<InodeState>(url, cache, totalSize);

    std::cout << "Prefetcher: Started prefetch for inode " << ino
              << " (size: " << totalSize << " bytes)" << std::endl;

    // Schedule initial sequential chunk from byte 0
    scheduleSequentialChunk(ino);
}

void Prefetcher::stopPrefetch(InodeNumber ino) {
    std::lock_guard<std::mutex> lock(m_mutex);

    auto it = m_states.find(ino);
    if (it != m_states.end()) {
        it->second->active = false;
        std::cout << "Prefetcher: Stopped prefetch for inode " << ino << std::endl;
        // Don't remove from map yet - let worker thread clean up tasks
    }
}

void Prefetcher::notifySeek(InodeNumber ino, int64_t offset) {
    std::lock_guard<std::mutex> lock(m_mutex);

    auto it = m_states.find(ino);
    if (it == m_states.end() || !it->second->active) {
        return;
    }

    InodeState* state = it->second.get();

    // Check if the seek target is already cached
    if (state->cache->isRangeCached(offset, kDefaultChunkSize)) {
        std::cout << "Prefetcher: Seek to " << offset << " already cached" << std::endl;
        return;
    }

    std::cout << "Prefetcher: Seek detected to offset " << offset
              << " (inode " << ino << "), scheduling high-priority fetch" << std::endl;

    // Create high-priority task for the seek target
    // Fetch multiple chunks ahead from the seek position
    int64_t remaining = state->totalSize - offset;
    int64_t fetchLength = std::min(kDefaultPrefetchAhead, remaining);

    PrefetchTask task;
    task.ino = ino;
    task.start = offset;
    task.length = fetchLength;
    task.priority = Priority::High;

    enqueuTask(task);
}

bool Prefetcher::isActive(InodeNumber ino) const {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_states.find(ino);
    return it != m_states.end() && it->second->active;
}

void Prefetcher::workerThread() {
    std::cout << "Prefetcher: Worker thread started" << std::endl;

    while (m_running) {
        PrefetchTask task;
        bool hasTask = false;

        // Wait for a task or shutdown signal
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_cv.wait(lock, [this] {
                return !m_taskQueue.empty() || !m_running;
            });

            if (!m_running && m_taskQueue.empty()) {
                break;
            }

            if (!m_taskQueue.empty()) {
                task = m_taskQueue.top();
                m_taskQueue.pop();
                hasTask = true;
            }
        }

        if (!hasTask) {
            continue;
        }

        // Process the task
        InodeState* state = nullptr;
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            auto it = m_states.find(task.ino);
            if (it == m_states.end() || !it->second->active) {
                // Inode was stopped, skip this task
                continue;
            }
            state = it->second.get();
        }

        // Check if the range is already cached (another task might have fetched it)
        if (state->cache->isRangeCached(task.start, task.length)) {
            // Already cached, schedule next sequential chunk if this was sequential
            if (task.priority == Priority::Low) {
                scheduleSequentialChunk(task.ino);
            }
            continue;
        }

        // Fetch the data
        std::cout << "Prefetcher: Fetching inode=" << task.ino
                  << " offset=" << task.start
                  << " length=" << task.length
                  << " priority=" << static_cast<int>(task.priority) << std::endl;

        // Break large requests into chunks
        int64_t remaining = task.length;
        int64_t currentOffset = task.start;

        while (remaining > 0 && state->active) {
            int64_t chunkSize = std::min(kDefaultChunkSize, remaining);

            // Check if this chunk is already cached
            if (!state->cache->isRangeCached(currentOffset, chunkSize)) {
                QByteArray data = m_fetcher->fetch(state->url, currentOffset, chunkSize);

                if (!data.isEmpty()) {
                    // Write to cache
                    state->cache->write(data.constData(), data.size(), currentOffset);
                    state->cache->markCached(currentOffset, data.size());

                    std::cout << "Prefetcher: Cached " << data.size()
                              << " bytes at offset " << currentOffset
                              << " (inode " << task.ino << ")" << std::endl;
                } else {
                    std::cerr << "Prefetcher: Failed to fetch data at offset "
                              << currentOffset << " (inode " << task.ino << ")" << std::endl;
                    // Don't retry immediately, break out
                    break;
                }
            }

            currentOffset += chunkSize;
            remaining -= chunkSize;
        }

        // If this was a sequential task, schedule the next chunk
        if (task.priority == Priority::Low && state->active) {
            scheduleSequentialChunk(task.ino);
        }
    }

    std::cout << "Prefetcher: Worker thread stopped" << std::endl;
}

void Prefetcher::enqueuTask(const PrefetchTask& task) {
    // Caller must hold m_mutex
    m_taskQueue.push(task);
    m_cv.notify_one();
}

void Prefetcher::scheduleSequentialChunk(InodeNumber ino) {
    // Caller must hold m_mutex
    auto it = m_states.find(ino);
    if (it == m_states.end() || !it->second->active) {
        return;
    }

    InodeState* state = it->second.get();
    int64_t currentPos = state->sequentialPos.load();

    // Don't prefetch beyond the file size
    if (currentPos >= state->totalSize) {
        std::cout << "Prefetcher: Sequential prefetch complete for inode " << ino << std::endl;
        return;
    }

    // Calculate how much to prefetch
    int64_t remaining = state->totalSize - currentPos;
    int64_t fetchLength = std::min(kDefaultChunkSize, remaining);

    // Create low-priority sequential task
    PrefetchTask task;
    task.ino = ino;
    task.start = currentPos;
    task.length = fetchLength;
    task.priority = Priority::Low;

    // Update sequential position
    state->sequentialPos.fetch_add(fetchLength);

    enqueuTask(task);
}

} // namespace kinetic
} // namespace mixxx

// MOC include for Qt metadata
#include "moc_prefetcher.cpp"
