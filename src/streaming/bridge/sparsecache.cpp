#include "streaming/bridge/sparsecache.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <algorithm>
#include <iostream>

SparseCache::SparseCache(const std::string& backingFilePath, int64_t totalSize)
        : m_totalSize(totalSize),
          m_backingFilePath(backingFilePath) {
    if (!m_backingFilePath.empty()) {
        // Create or open the backing file
        int fd = open(m_backingFilePath.c_str(), O_RDWR | O_CREAT, 0644);
        if (fd >= 0) {
            // Pre-allocate space if possible (optional but good for performance)
            if (m_totalSize > 0) {
                if (ftruncate(fd, m_totalSize) != 0) {
                    std::cerr << "SparseCache: Failed to truncate backing file" << std::endl;
                }
            }
            close(fd);
        } else {
            std::cerr << "SparseCache: Failed to open backing file: " << m_backingFilePath << std::endl;
        }
    }
}

bool SparseCache::isRangeCached(int64_t start, int64_t length) const {
    if (length <= 0) {
        return true;
    }
    int64_t end = start + length;

    // Find the first interval that starts after our start point
    auto it = m_intervals.upper_bound(start);

    // Check key before (it might cover our start)
    if (it != m_intervals.begin()) {
        auto prev = std::prev(it);
        // If prev covers our start
        if (prev->first <= start && prev->second >= end) {
            return true;
        }
    }

    return false;
}

std::vector<SparseCache::CachedRange> SparseCache::getMissingRanges(int64_t start, int64_t length) const {
    std::vector<CachedRange> missing;
    if (length <= 0) {
        return missing;
    }

    int64_t current = start;
    int64_t requestEnd = start + length;

    auto it = m_intervals.lower_bound(start);
    // Move back one if potential overlap
    if (it != m_intervals.begin()) {
        auto prev = std::prev(it);
        if (prev->second > current) {
            it = prev;
        }
    }

    while (current < requestEnd) {
        // No more cached intervals
        if (it == m_intervals.end()) {
            missing.push_back({current, requestEnd});
            break;
        }

        int64_t cachedStart = it->first;
        int64_t cachedEnd = it->second;

        if (cachedStart > current) {
            // Gap between current and next cached block
            int64_t gapEnd = std::min(cachedStart, requestEnd);
            missing.push_back({current, gapEnd});
            current = gapEnd;
        }

        // We are strictly inside or effectively past this block now?
        // If current < cachedEnd, we skip the cached part
        if (current < cachedEnd) {
            current = cachedEnd;
        }

        ++it;
    }

    return missing;
}

void SparseCache::markCached(int64_t start, int64_t length) {
    if (length <= 0) {
        return;
    }
    int64_t end = start + length;
    m_intervals[start] = end; // Insert new interval
    mergeOverlappingIntervals();
}

double SparseCache::getCachedPercentage() const {
    if (m_totalSize <= 0) {
        return 0.0;
    }

    int64_t cachedBytes = 0;
    for (const auto& [start, end] : m_intervals) {
        cachedBytes += (end - start);
    }

    return (double)cachedBytes / m_totalSize * 100.0;
}

std::vector<SparseCache::CachedRange> SparseCache::getAllCachedRanges() const {
    std::vector<CachedRange> ranges;
    ranges.reserve(m_intervals.size());
    for (const auto& [start, end] : m_intervals) {
        ranges.push_back({start, end});
    }
    return ranges;
}

ssize_t SparseCache::read(char* buffer, size_t size, off_t offset) {
    if (m_backingFilePath.empty()) {
        return -1;
    }

    int fd = open(m_backingFilePath.c_str(), O_RDONLY);
    if (fd < 0) {
        return -1;
    }

    // pread is thread-safe for reading from offset
    ssize_t bytesRead = pread(fd, buffer, size, offset);
    close(fd);
    return bytesRead;
}

ssize_t SparseCache::write(const char* buffer, size_t size, off_t offset) {
    if (m_backingFilePath.empty()) {
        return -1;
    }

    int fd = open(m_backingFilePath.c_str(), O_WRONLY);
    if (fd < 0) {
        return -1;
    }

    ssize_t bytesWritten = pwrite(fd, buffer, size, offset);
    close(fd);

    if (bytesWritten > 0) {
        markCached(offset, bytesWritten);
    }

    return bytesWritten;
}

void SparseCache::mergeOverlappingIntervals() {
    if (m_intervals.empty()) {
        return;
    }

    auto it = m_intervals.begin();
    while (it != m_intervals.end()) {
        auto next = std::next(it);
        if (next == m_intervals.end()) {
            break;
        }

        if (it->second >= next->first) {
            // Overlap or adjacency
            it->second = std::max(it->second, next->second);
            m_intervals.erase(next);
            // Don't increment 'it', check the new 'next' against the grown 'it'
        } else {
            ++it;
        }
    }
}
