#include "sources/streamringbuffer.h"

#include <gtest/gtest.h>

#include <atomic>
#include <thread>
#include <vector>

namespace mixxx {

TEST(StreamRingBufferTest, BasicReadWrite) {
    StreamRingBuffer<int> buffer(10);

    std::vector<int> input = {1, 2, 3, 4, 5};
    size_t written = buffer.write(input.data(), input.size());
    EXPECT_EQ(written, 5);
    EXPECT_FALSE(buffer.empty());

    std::vector<int> output(5);
    size_t read = buffer.read(output.data(), 5);
    EXPECT_EQ(read, 5);
    EXPECT_EQ(output, input);
    EXPECT_TRUE(buffer.empty());
}

TEST(StreamRingBufferTest, OverflowHandling) {
    StreamRingBuffer<int> buffer(5);

    std::vector<int> input = {1, 2, 3, 4, 5, 6, 7};
    size_t written = buffer.write(input.data(), input.size());
    EXPECT_EQ(written, 5); // Capacity is slightly larger usually due to internal impl, but let's check basic
    // actually rigtorp impl usually reserves capacity+1 or similar, but let's trust it stops when full

    std::vector<int> output(5);
    size_t read = buffer.read(output.data(), 5);
    EXPECT_EQ(read, 5);
    EXPECT_EQ(output[0], 1);
    EXPECT_EQ(output[4], 5);
}

TEST(StreamRingBufferTest, ConcurrentProducerConsumer) {
    const size_t kCount = 100000;
    StreamRingBuffer<int> buffer(1024);

    std::atomic<bool> producerDone(false);

    std::thread producer([&]() {
        for (int i = 0; i < (int)kCount; ++i) {
            while (buffer.write(&i, 1) == 0) {
                std::this_thread::yield();
            }
        }
        producerDone = true;
    });

    std::thread consumer([&]() {
        int expected = 0;
        while (expected < (int)kCount) {
            int val;
            if (buffer.read(&val, 1) > 0) {
                EXPECT_EQ(val, expected);
                expected++;
            } else {
                if (producerDone && buffer.empty())
                    break;
                std::this_thread::yield();
            }
        }
    });

    producer.join();
    consumer.join();
}

} // namespace mixxx
