#pragma once

#include <QUrl>
#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

// Forward declarations
class SparseCache;

namespace mixxx {
namespace kinetic {

class RangeFetcher;

/// Type alias for FUSE inode numbers (avoiding direct fuse.h dependency)
using InodeNumber = uint64_t;

/// @brief Intelligent background prefetcher for streaming audio files
/// Implements the "Zero-Trust Stream Principle" by proactively downloading
/// data ahead of playback to prevent buffer underruns.
///
/// Features:
/// - Sequential prefetching from byte 0 when file opens
/// - Priority prefetching when seeks are detected
/// - Single background worker thread managing all active streams
/// - Cache-aware: checks SparseCache before issuing HTTP requests
class Prefetcher {
  public:
    /// Default chunk size for prefetch operations (256KB)
    /// Balance between HTTP request overhead and seek granularity
    static constexpr int64_t kDefaultChunkSize = 256 * 1024;

    /// How far ahead to prefetch sequentially (in bytes)
    /// Default: 30 seconds @ 256kbps AAC ≈ 960KB, rounded to 1MB
    static constexpr int64_t kDefaultPrefetchAhead = 1024 * 1024;

    enum class Priority {
        Low = 0,   // Sequential background prefetch
        High = 10, // User-initiated seek (needs immediate attention)
    };

    struct PrefetchTask {
        InodeNumber ino;
        int64_t start;
        int64_t length;
        Priority priority;

        // Comparison for priority queue (higher priority = processed first)
        bool operator<(const PrefetchTask& other) const {
            // Note: std::priority_queue is a max-heap, so we invert comparison
            if (priority != other.priority) {
                return static_cast<int>(priority) < static_cast<int>(other.priority);
            }
            // Within same priority, prefer earlier offsets
            return start > other.start;
        }
    };

    explicit Prefetcher(RangeFetcher* fetcher);
    ~Prefetcher();

    /// Start prefetching for a new file
    /// @param ino FUSE inode number
    /// @param url Stream URL
    /// @param cache SparseCache instance for this file
    /// @param totalSize Total file size in bytes
    void startPrefetch(InodeNumber ino,
            const QUrl& url,
            SparseCache* cache,
            int64_t totalSize);

    /// Stop prefetching for a file (e.g., when file is closed or fully cached)
    /// @param ino FUSE inode number
    void stopPrefetch(InodeNumber ino);

    /// Notify prefetcher of a seek operation
    /// This creates a high-priority task to prefetch from the seek position
    /// @param ino FUSE inode number
    /// @param offset Seek target offset in bytes
    void notifySeek(InodeNumber ino, int64_t offset);

    /// Check if prefetcher is active for a given inode
    bool isActive(InodeNumber ino) const;

  private:
    struct InodeState {
        QUrl url;
        SparseCache* cache; // Not owned
        int64_t totalSize;
        std::atomic<int64_t> sequentialPos; // Current sequential prefetch position
        std::atomic<bool> active;

        InodeState(const QUrl& u, SparseCache* c, int64_t size)
                : url(u),
                  cache(c),
                  totalSize(size),
                  sequentialPos(0),
                  active(true) {
        }
    };

    // Background worker thread entry point
    void workerThread();

    // Enqueue a prefetch task
    void enqueuTask(const PrefetchTask& task);

    // Schedule next sequential chunk for an inode
    void scheduleSequentialChunk(InodeNumber ino);

    RangeFetcher* m_fetcher; // Not owned
    std::thread m_worker;
    std::atomic<bool> m_running;

    mutable std::mutex m_mutex;
    std::condition_variable m_cv;
    std::priority_queue<PrefetchTask> m_taskQueue;
    std::map<InodeNumber, std::unique_ptr<InodeState>> m_states;
};

} // namespace kinetic
} // namespace mixxx
