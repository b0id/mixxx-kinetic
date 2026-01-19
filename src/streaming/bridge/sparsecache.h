#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <vector>

class SparseCache {
  public:
    struct CachedRange {
        int64_t start;
        int64_t end; // exclusive
    };

    SparseCache(const std::string& backingFilePath, int64_t totalSize);

    // Check if byte range is fully cached
    bool isRangeCached(int64_t start, int64_t length) const;

    // Get list of missing ranges within a request
    std::vector<CachedRange> getMissingRanges(int64_t start, int64_t length) const;

    // Mark range as cached after download
    void markCached(int64_t start, int64_t length);

    // File I/O
    ssize_t read(char* buffer, size_t size, off_t offset);
    ssize_t write(const char* buffer, size_t size, off_t offset);

    // Get total cached percentage
    double getCachedPercentage() const;

    // Get total size
    int64_t getTotalSize() const {
        return m_totalSize;
    }

    // Get all cached ranges (for waveform rendering)
    std::vector<CachedRange> getAllCachedRanges() const;

  private:
    std::string m_backingFilePath;
    int64_t m_totalSize;
    std::map<int64_t, int64_t> m_intervals; // start -> end

    void mergeOverlappingIntervals();
};
