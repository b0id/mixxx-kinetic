#include "fusedriver.h"

#include <fuse_lowlevel.h>
#include <sys/stat.h>

#include <cstring>
#include <filesystem>
#include <iostream>

FuseDriver* FuseDriver::s_instance = nullptr;

FuseDriver::FuseDriver() {
    s_instance = this;
}

FuseDriver::~FuseDriver() {
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
    m_nameToInode[filename] = ino;
    std::cout << "FuseDriver: Registered " << filename << " -> " << ino << std::endl;

    m_nameToInode[filename] = ino;
    std::cout << "FuseDriver: Registered " << filename << " -> " << ino << std::endl;

    return ino;
}

void FuseDriver::unregisterFile(fuse_ino_t ino) {
    std::lock_guard<std::mutex> lock(m_mutex);

    auto it = m_caches.find(ino);
    if (it != m_caches.end()) {
        delete it->second;
        m_caches.erase(it);

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

    // Check if range is cached
    // For now, since we implemented file backing read only if cached,
    // we should be careful.
    // But SparseCache::read reads from the backing file.
    // If the backing file doesn't have data, it will read zeroes or garbage unless we ensure it's written.

    // For testing, let's assume we want to read whatever is in the backing file.
    // But real implementation needs to check isRangeCached and block/download.

    char* buf = new char[size];
    ssize_t res = cache->read(buf, size, off);

    if (res >= 0) {
        fuse_reply_buf(req, buf, res);
    } else {
        fuse_reply_err(req, EIO);
    }

    delete[] buf;
}
