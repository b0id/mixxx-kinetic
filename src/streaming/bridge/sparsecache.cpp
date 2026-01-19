#include "sparsecache.h"

SparseCache::SparseCache()
        : m_totalSize(0) {
}

bool SparseCache::isRangeCached(int64_t start, int64_t length) const {
    return false;
}

std::vector<SparseCache::CachedRange> SparseCache::getMissingRanges(int64_t start, int64_t length) const {
    return {{start, start + length}};
}

void SparseCache::markCached(int64_t start, int64_t length) {
    // Stub
}

double SparseCache::getCachedPercentage() const {
    return 0.0;
}

std::vector<SparseCache::CachedRange> SparseCache::getAllCachedRanges() const {
    return {};
}

void SparseCache::mergeOverlappingIntervals() {
    // Stub
}
