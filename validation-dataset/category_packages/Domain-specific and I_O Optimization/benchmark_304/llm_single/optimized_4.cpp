#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>
#include <atomic>

constexpr int GCINTERVAL = 10; // ms

static std::mutex wake_mutex;
static std::condition_variable wake_cv;
static std::atomic<bool> stop_flag{false};
static std::vector<int> maybe_free;

// Garbage collector thread: sleeps for one second between passes
static void distref_thread() {
    while (!stop_flag.load(std::memory_order_relaxed)) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::lock_guard<std::mutex> lk(wake_mutex);
        maybe_free.clear();
    }
}

long long distref_simulate(int iterations) {
    stop_flag.store(false, std::memory_order_relaxed);
    std::thread t(distref_thread);
    long long result = 0;
    std::vector<int> local_maybe_free;
    local_maybe_free.reserve(iterations);
    for (int i = 0; i < iterations; ++i) {
        {
            std::lock_guard<std::mutex> lk(wake_mutex);
            local_maybe_free.push_back(i);
            // Use a reference to avoid potential performance hit from too many dereferences
            volatile int dummy = 0;
            for (const auto& v : local_maybe_free)
                dummy |= v;
        }
        result += i;
    }
    stop_flag.store(true, std::memory_order_relaxed);
    wake_cv.notify_all();
    t.join();
    return result;
}
