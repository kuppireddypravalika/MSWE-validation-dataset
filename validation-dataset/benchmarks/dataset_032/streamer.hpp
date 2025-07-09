#pragma once
#include <chrono>
#include <thread>
#include <mutex>

class MockBio {
public:
    explicit MockBio(bool active = true) : active_(active) {}
    bool isactive() const { return active_; }
    bool cansend(uint32_t timeout_us) {
        std::this_thread::sleep_for(std::chrono::microseconds(timeout_us));
        return true;
    }
private:
    bool active_;
};

class Streamer {
public:
    explicit Streamer(MockBio* bio) : mBio(bio), sent(0) {}

    int tick_send(uint32_t timeout);

    void free_pend() {}
    void handleoutgoing_locked() { ++sent; }

    std::size_t sent;
private:
    std::mutex mStreamerMtx;
    MockBio* mBio;
};
