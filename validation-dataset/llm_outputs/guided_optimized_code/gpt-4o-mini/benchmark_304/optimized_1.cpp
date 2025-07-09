#include <atomic>
#include <chrono>
#include <thread>
#include <vector>

constexpr int GCINTERVAL_MS = 1000; // milliseconds (1 second)

static std::atomic<bool> stop_flag{false};
static std::vector<int> maybe_free;

// Garbage collector thread: sleeps for one second between passes
static void distref_thread() {
    while (!stop_flag.load(std::memory_order_relaxed)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(GCINTERVAL_MS));
        maybe_free.clear();  // Clear the vector every second
    }
}

long long distref_simulate(int iterations) {
    stop_flag.store(false, std::memory_order_relaxed);
    std::thread t(distref_thread);
    long long result = 0;

    // Reserve memory for maybe_free in advance to reduce reallocations
    maybe_free.reserve(iterations);

    for (int i = 0; i < iterations; ++i) {
        {
            maybe_free.push_back(i);  // Add current index to vector
        }
        result += i;
    }

    stop_flag.store(true, std::memory_order_relaxed);
    t.join(); // Wait for the garbage collector thread to finish
    return result;
}

// Explicit template instantiation statements if necessary:
// template class YourTemplateClass<int>;
// explicit template instantiation of your functions or classes here
