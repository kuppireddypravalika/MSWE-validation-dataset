#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>
#include <atomic>

constexpr int GCINTERVAL = 10; // ms

static std::atomic<bool> stop_flag{false};
static std::vector<int> maybe_free;

// Garbage collector thread: sleeps for a second between passes
static void distref_thread() {
    while (!stop_flag.load(std::memory_order_relaxed)) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        maybe_free.clear(); // Clear contents periodically
    }
}

long long distref_simulate(int iterations) {
    stop_flag.store(false, std::memory_order_relaxed);
    std::thread t(distref_thread);
    long long result = 0;
    int running_dummy = 0; // Variable to maintain the current OR result
    maybe_free.reserve(iterations); // Reserve size in advance

    for (int i = 0; i < iterations; ++i) {
        // Add i to maybe_free
        maybe_free.push_back(i);
        running_dummy |= i; // Update running_dummy with the current index
        result += i; // Accumulate the result
    }

    stop_flag.store(true, std::memory_order_relaxed);
    t.join(); // Wait for the garbage collector to finish
    return result;
}

// Explicitly instantiate templates if any
// For example, if there are specific template functions needed
// template class Example<int>;
// template class Example<double>;