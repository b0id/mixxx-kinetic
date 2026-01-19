#include "fusedriver.h"

#include <iostream>

FuseDriver::FuseDriver() {
}

FuseDriver::~FuseDriver() {
}

void FuseDriver::init(void* userdata, struct fuse_conn_info* conn) {
    std::cout << "FuseDriver::init" << std::endl;
}

void FuseDriver::getattr(fuse_req_t req, fuse_ino_t ino, struct fuse_file_info* fi) {
    // Stub
    fuse_reply_err(req, ENOENT);
}

void FuseDriver::lookup(fuse_req_t req, fuse_ino_t parent, const char* name) {
    fuse_reply_err(req, ENOENT);
}

void FuseDriver::open(fuse_req_t req, fuse_ino_t ino, struct fuse_file_info* fi) {
    fuse_reply_err(req, ENOENT);
}

void FuseDriver::read(fuse_req_t req, fuse_ino_t ino, size_t size, off_t off, struct fuse_file_info* fi) {
    fuse_reply_err(req, EIO);
}
