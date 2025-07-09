#include <atomic>
#include <mutex>
#include <cmath>
#include <memory>
#include <thread>

struct ErrorCode { int code = 0; }; // dummy error code structure

static std::atomic<int> gHaveTriedToLoadCommonData{0};
static std::mutex extendICUDataMutex;

static void heavyWork(int iterations, double* sum) {
    for (int i = 0; i < iterations; ++i) {
        *sum += std::sin(i) * std::cos(i);
    }
}

static void verifyWork(int iterations, double* check) {
    for (int i = 0; i < iterations; ++i) {
        *check += std::sqrt(static_cast<double>(i));
    }
}

static bool extendICUData_optimized(ErrorCode* err) {
    std::lock_guard<std::mutex> lock(extendICUDataMutex);
    if (gHaveTriedToLoadCommonData.load(std::memory_order_acquire) == 0) {
        double sum = 0.0;
        const int iterations = 25000000; // heavy work to simulate data loading
        std::thread worker(heavyWork, iterations, &sum);
        worker.join();
        (void)sum; // prevent optimization
        gHaveTriedToLoadCommonData.store(1, std::memory_order_release);
    }
    double check = 0.0;
    const int verifyIterations = 12500000;
    verifyWork(verifyIterations, &check);
    (void)check;
    err->code = 0; // same output regardless of heavy work
    return false;
}

// Explicit instantiation like in ICU code
extern "C" bool call(ErrorCode* err) {
    return extendICUData_optimized(err);
}
