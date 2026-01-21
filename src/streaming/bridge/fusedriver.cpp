#include "fusedriver.h"

#include <fuse_lowlevel.h>
#include <sys/stat.h>

#include <cstring>
#include <filesystem>
#include <iostream>

FuseDriver* FuseDriver::s_instance = nullptr;

FuseDriver::FuseDriver()
        : m_fetcher(new mixxx::kinetic::RangeFetcher(nullptr)),
          m_prefetcher(new mixxx::kinetic::Prefetcher(m_fetcher)) {
    s_instance = this;
}

FuseDriver::~FuseDriver() {
    // Delete prefetcher first (stops worker threads)
    delete m_prefetcher;

    // Delete fetcher
    delete m_fetcher;

    for (auto const& [ino, cache] : m_caches) {
        delete cache;
    }
    m_caches.clear();
    if (s_instance == this) {
        s_instance = nullptr;
    }
}

FuseDriver* FuseDriver::instance() {
    return s_instance;
}

SparseCache* FuseDriver::getCache(fuse_ino_t ino) {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_caches.find(ino);
    if (it != m_caches.end()) {
        return it->second;
    }
    return nullptr;
}

fuse_ino_t FuseDriver::registerFile(const std::string& backingPath, int64_t size) {
    std::lock_guard<std::mutex> lock(m_mutex);
    // Inode allocation strategy:
    // For now simple counter. Root=1, Test=2.
    static fuse_ino_t nextInode = 3;
    fuse_ino_t ino = nextInode++;

    m_caches[ino] = new SparseCache(backingPath, size);

    // Store filename mapping
    std::string filename = std::filesystem::path(backingPath).filename().string();

    // Check if backingPath is a URL (hacky check for now)
    if (backingPath.find("http") == 0) {
        // It's a URL!
        m_inodeToUrl[ino] = backingPath;
        // filename might be slashes or ugly, let's just use inode ID for now if it looks like a URL
        filename = std::to_string(ino) + ".mp3";
    }

    m_nameToInode[filename] = ino;
    std::cout << "FuseDriver: Registered " << filename << " -> " << ino << std::endl;

    // Start prefetching if this is a URL
    if (backingPath.find("http") == 0) {
        QUrl url(QString::fromStdString(backingPath));
        m_prefetcher->startPrefetch(ino, url, m_caches[ino], size);
        std::cout << "FuseDriver: Started prefetch for " << filename << std::endl;
    }

    return ino;
}

void FuseDriver::unregisterFile(fuse_ino_t ino) {
    std::lock_guard<std::mutex> lock(m_mutex);

    // Stop prefetching
    m_prefetcher->stopPrefetch(ino);

    auto it = m_caches.find(ino);
    if (it != m_caches.end()) {
        delete it->second;
        m_caches.erase(it);
        m_inodeToUrl.erase(ino);

        // Remove from name map (slow linear scan, but acceptable for now)
        for (auto nit = m_nameToInode.begin(); nit != m_nameToInode.end();) {
            if (nit->second == ino) {
                std::cout << "FuseDriver: Unregistered " << nit->first << " -> " << ino << std::endl;
                nit = m_nameToInode.erase(nit);
            } else {
                ++nit;
            }
        }
    }
}

void FuseDriver::createTestFile(fuse_ino_t ino) {
    std::lock_guard<std::mutex> lock(m_mutex);
    // Create a 10MB virtual test file backed by /tmp/mixxx-cache-test
    m_caches[ino] = new SparseCache("/tmp/mixxx-cache-test.mp3", 10 * 1024 * 1024);
}

void FuseDriver::init(void* userdata, struct fuse_conn_info* conn) {
    (void)userdata;
    (void)conn;
    std::cout << "FuseDriver::init: Connection established" << std::endl;

    // Create the global singleton if not already exists (though constructor should handle it)
    // In our main.cpp we don't explicitly create FuseDriver instance yet?
    // We need to ensure an instance exists.
    // For now, let's create a test file on init for inode 2
    if (instance()) {
        instance()->createTestFile(2);
        std::cout << "FuseDriver::init: Created test file at inode 2" << std::endl;
    } else {
        std::cerr << "FuseDriver::init: No instance available!" << std::endl;
    }
}

void FuseDriver::getattr(fuse_req_t req, fuse_ino_t ino, struct fuse_file_info* fi) {
    (void)fi; // Unused
    struct stat stbuf;
    memset(&stbuf, 0, sizeof(stbuf));

    if (ino == 1) { // Root directory
        stbuf.st_mode = S_IFDIR | 0755;
        stbuf.st_nlink = 2;
        fuse_reply_attr(req, &stbuf, 1.0);
    } else if (ino == 2) { // Test file
        stbuf.st_mode = S_IFREG | 0444;
        stbuf.st_nlink = 1;
        stbuf.st_size = 10 * 1024 * 1024; // 10MB
        fuse_reply_attr(req, &stbuf, 1.0);
    } else {
        fuse_reply_err(req, ENOENT);
    }
}

void FuseDriver::lookup(fuse_req_t req, fuse_ino_t parent, const char* name) {
    struct fuse_entry_param e;
    memset(&e, 0, sizeof(e));

    if (parent == 1) {
        fuse_ino_t foundIno = 0;
        size_t fileSize = 0;

        // Static test file
        if (strcmp(name, "test.mp3") == 0) {
            foundIno = 2;
            fileSize = 10 * 1024 * 1024;
        } else {
            // Check dynamic map
            // Use instance() to access non-static member
            if (instance()) {
                std::lock_guard<std::mutex> lock(instance()->m_mutex);
                auto it = instance()->m_nameToInode.find(name);
                if (it != instance()->m_nameToInode.end()) {
                    foundIno = it->second;
                    // Get size from cache
                    auto cacheIt = instance()->m_caches.find(foundIno);
                    if (cacheIt != instance()->m_caches.end()) {
                        fileSize = cacheIt->second->getTotalSize();
                    }
                }
            }
        }

        if (foundIno > 0) {
            e.ino = foundIno;
            e.attr_timeout = 1.0;
            e.entry_timeout = 1.0;
            e.attr.st_ino = e.ino;
            e.attr.st_mode = S_IFREG | 0444;
            e.attr.st_nlink = 1;
            e.attr.st_size = fileSize;
            fuse_reply_entry(req, &e);
            return;
        }
    }

    fuse_reply_err(req, ENOENT);
}

void FuseDriver::open(fuse_req_t req, fuse_ino_t ino, struct fuse_file_info* fi) {
    if (instance() && instance()->getCache(ino)) {
        fuse_reply_open(req, fi);
    } else {
        fuse_reply_err(req, ENOENT);
    }
}

void FuseDriver::read(fuse_req_t req, fuse_ino_t ino, size_t size, off_t off, struct fuse_file_info* fi) {
    (void)fi;
    if (!instance()) {
        fuse_reply_err(req, EIO);
        return;
    }

    SparseCache* cache = instance()->getCache(ino);
    if (!cache) {
        fuse_reply_err(req, ENOENT);
        return;
    }

    // Check if we have a URL for this inode (streaming file)
    std::string urlStr;
    {
        std::lock_guard<std::mutex> lock(instance()->m_mutex);
        auto it = instance()->m_inodeToUrl.find(ino);
        if (it != instance()->m_inodeToUrl.end()) {
            urlStr = it->second;
        }
    }

    // If this is a URL-backed file and the range is not cached
    if (!urlStr.empty() && !cache->isRangeCached(off, size)) {
        // Notify prefetcher of potential seek
        // The prefetcher will prioritize this range if it's far from current sequential position
        instance()->m_prefetcher->notifySeek(ino, off);

        // Perform synchronous fallback fetch for this specific range
        // This ensures the read completes even if prefetcher hasn't caught up yet
        std::cout << "FuseDriver::read: Cache miss at offset " << off
                  << ", performing fallback fetch (inode " << ino << ")" << std::endl;

        QByteArray data = instance()->m_fetcher->fetch(
                QUrl(QString::fromStdString(urlStr)), off, size);
        if (!data.isEmpty()) {
            cache->write(data.constData(), data.size(), off);
            cache->markCached(off, data.size());
        } else {
            std::cerr << "FuseDriver::read: Fallback fetch failed!" << std::endl;
            fuse_reply_err(req, EIO);
            return;
        }
    }

    // Read from cache (either was already cached or just fetched)
    char* buf = new char[size];
    ssize_t res = cache->read(buf, size, off);

    if (res >= 0) {
        fuse_reply_buf(req, buf, res);
    } else {
        fuse_reply_err(req, EIO);
    }

    delete[] buf;
}
