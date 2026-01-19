#pragma once

#include <fuse_lowlevel.h>

#include <string>

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

  private:
    // SparseCache* m_cache;
};
