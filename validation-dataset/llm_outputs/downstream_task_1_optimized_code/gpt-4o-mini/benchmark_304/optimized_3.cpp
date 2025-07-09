#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>
#include <atomic>
#include <algorithm>

constexpr int GCINTERVAL = 10; // ms

static std::mutex wake_mutex;
static std::condition_variable wake_cv;
static std::atomic<bool> stop_flag(false);
static std::vector<int> maybe_free;

// Garbage collector thread: sleeps for one second between passes
static void distref_thread() {
    while (!stop_flag.load()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::lock_guard<std::mutex> lk(wake_mutex);
        maybe_free.clear();
    }
}

long long distref_simulate(int iterations) {
    stop_flag.store(false);
    std::thread t(distref_thread);
    long long result = 0;
    maybe_free.reserve(iterations); // Preallocate to avoid reallocations
    for (int i = 0; i < iterations; ++i) {
        {
            std::lock_guard<std::mutex> lk(wake_mutex);
            maybe_free.push_back(i);
            // Use std::accumulate to avoid the loop overhead
            volatile int dummy = std::accumulate(maybe_free.begin(), maybe_free.end(), 0, std::bit_or<int>());
        }
        result += i;
    }
    stop_flag.store(true);
    wake_cv.notify_all();
    t.join();
    return result;
}

// Explicit template instantiation statements
// Add your explicit template instantiation statements here if any