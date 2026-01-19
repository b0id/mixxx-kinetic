#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

#include "streaming/bridge/fusedriver.h"

// Mock RangeFetcher?
// Since we can't easily mock the private fetcher in the singleton,
// we might test the end-to-end flow with a real URL or skip the network part if we want offline tests.
// For now, let's verify FuseDriver Logic accepts URL registration.

TEST(FuseDriverTest, RegisterUrl) {
    if (FuseDriver::instance())
        delete FuseDriver::instance();
    FuseDriver driver;

    std::string url = "http://localhost:8080/stream.mp3";
    fuse_ino_t ino = driver.registerFile(url, 1024);

    EXPECT_GT(ino, 2);

    // Cleanup
    driver.unregisterFile(ino);
}
