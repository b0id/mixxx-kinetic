#pragma once

#include <fuse_lowlevel.h>

#include <map>
#include <mutex>
#include <string>

#include "streaming/bridge/sparsecache.h"

class FuseDriver {
  public:
    FuseDriver();
    virtual ~FuseDriver();

    // FUSE operations
    // Note: These will be static wrappers calling into the instance
    static void init(void* userdata, struct fuse_conn_info* conn);
    static void getattr(fuse_req_t req, fuse_ino_t ino, struct fuse_file_info* fi);
    static void lookup(fuse_req_t req, fuse_ino_t parent, const char* name);
    static void open(fuse_req_t req, fuse_ino_t ino, struct fuse_file_info* fi);
    static void read(fuse_req_t req, fuse_ino_t ino, size_t size, off_t off, struct fuse_file_info* fi);
    // ... other ops ...

    // Singleton instance access for callbacks
    static FuseDriver* instance();

    // SparseCache management
    SparseCache* getCache(fuse_ino_t ino);
    fuse_ino_t registerFile(const std::string& backingPath, int64_t size);
    void unregisterFile(fuse_ino_t ino);
    void createTestFile(fuse_ino_t ino); // Helper for development testing

  private:
    static FuseDriver* s_instance;
    // Map inode to SparseCache
    // For now, effectively owning the pointers.
    // In real implementatoin, these would be managed by a more complex lifecycle
    std::map<fuse_ino_t, SparseCache*> m_caches;
    std::map<std::string, fuse_ino_t> m_nameToInode; // Filename in root -> Inode
    std::mutex m_mutex;
};
