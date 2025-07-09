#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

constexpr int GCINTERVAL_MS = 1000; // milliseconds (1 second)

static std::atomic<bool> stop_flag{false};
static std::vector<int> maybe_free;

// Garbage collector thread: sleeps for one second between passes
static void distref_thread() {
    while (!stop_flag.load(std::memory_order_relaxed)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(GCINTERVAL_MS));
        maybe_free.clear();
    }
}

long long distref_simulate(int iterations) {
    stop_flag.store(false, std::memory_order_relaxed);
    std::thread t(distref_thread);
    long long result = 0;
    int running_dummy = 0; // Variable to maintain the current OR result
    maybe_free.reserve(iterations); // Reserve space based on expected usage

    for (int i = 0; i < iterations; ++i) {
        // Push back and update running_dummy
        {
            maybe_free.push_back(i);
            running_dummy |= i;
        }
        result += i;
    }

    stop_flag.store(true, std::memory_order_relaxed);
    t.join();
    return result;
}

// Explicit template instantiation statements (if any) would go here.