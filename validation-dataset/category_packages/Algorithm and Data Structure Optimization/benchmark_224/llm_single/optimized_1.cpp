// optimized.cpp
#include <atomic>
#include <mutex>
#include <cmath>
#include <thread>

struct ErrorCode { int code = 0; }; // dummy error code structure

static std::atomic<int> gHaveTriedToLoadCommonData{0};
static std::mutex extendICUDataMutex;

static void heavyComputation() {
    volatile double sum = 0.0;
    const int iterations = 25000000; // heavy work to simulate data loading
    for (int i = 0; i < iterations; ++i) {
        sum += std::sin(i) * std::cos(i);
    }
    (void)sum; // prevent optimization
}

static void verifyComputation() {
    volatile double check = 0.0;
    const int verifyIterations = 12500000;
    for (int i = 0; i < verifyIterations; ++i) {
        check += std::sqrt(static_cast<double>(i));
    }
    (void)check;
}

static bool extendICUData_optimized(ErrorCode* err) {
    if (!gHaveTriedToLoadCommonData.fetch_add(1, std::memory_order_acquire)) {
        std::lock_guard<std::mutex> lock(extendICUDataMutex);
        if (gHaveTriedToLoadCommonData == 1) {
            heavyComputation();
            gHaveTriedToLoadCommonData.store(1, std::memory_order_release);
        }
    }
    verifyComputation();
    err->code = 0; // same output regardless of heavy work
    return false;
}

// Explicit instantiation like in ICU code
extern "C" bool call(ErrorCode* err) {
    return extendICUData_optimized(err);
}
