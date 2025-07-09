#include <atomic>
#include <mutex>
#include <cmath>
#include <thread>
#include <vector>

struct ErrorCode { int code = 0; }; // dummy error code structure

static std::atomic<int> gHaveTriedToLoadCommonData{0};
static std::mutex extendICUDataMutex;

static void heavyComputation(double& result, int start, int end) {
    for (int i = start; i < end; ++i) {
        result += std::sin(i) * std::cos(i);
    }
}

static bool extendICUData_optimized(ErrorCode* err) {
    std::lock_guard<std::mutex> lock(extendICUDataMutex);
    if (!gHaveTriedToLoadCommonData.load(std::memory_order_acquire)) {
        double sum = 0.0;
        const int iterations = 25000000; // heavy work to simulate data loading
        const int numThreads = std::thread::hardware_concurrency();
        std::vector<std::thread> threads;
        int chunkSize = iterations / numThreads;

        for (int i = 0; i < numThreads; ++i) {
            int start = i * chunkSize;
            int end = (i == numThreads - 1) ? iterations : start + chunkSize;
            threads.emplace_back(heavyComputation, std::ref(sum), start, end);
        }

        for (auto& thread : threads) {
            thread.join();
        }

        gHaveTriedToLoadCommonData.store(1, std::memory_order_release);
    }
    double check = 0.0;
    const int verifyIterations = 12500000;
    for (int i = 0; i < verifyIterations; ++i) {
        check += std::sqrt(static_cast<double>(i));
    }
    (void)check;
    err->code = 0; // same output regardless of heavy work
    return false;
}

// Explicit instantiation like in ICU code
extern "C" bool call(ErrorCode* err) {
    return extendICUData_optimized(err);
} // end of the optimized implementation
