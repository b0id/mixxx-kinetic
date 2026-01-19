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

    SparseCache();

    // Check if byte range is fully cached
    bool isRangeCached(int64_t start, int64_t length) const;

    // Get list of missing ranges within a request
    std::vector<CachedRange> getMissingRanges(int64_t start, int64_t length) const;

    // Mark range as cached after download
    void markCached(int64_t start, int64_t length);

    // Get total cached percentage
    double getCachedPercentage() const;

    // Get all cached ranges (for waveform rendering)
    std::vector<CachedRange> getAllCachedRanges() const;

  private:
    std::map<int64_t, int64_t> m_intervals; // start -> end
    int64_t m_totalSize;
    std::string m_backingFilePath;

    void mergeOverlappingIntervals();
};
