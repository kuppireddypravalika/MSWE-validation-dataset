#include <atomic>
#include <mutex>
#include <cmath>

struct ErrorCode { int code = 0; }; // dummy error code structure

static std::atomic<int> gHaveTriedToLoadCommonData{0};
static std::mutex extendICUDataMutex;

static bool extendICUData(ErrorCode* err) {
    if (gHaveTriedToLoadCommonData.load(std::memory_order_acquire) == 0) {
        std::lock_guard<std::mutex> lock(extendICUDataMutex);

        // Check again within the locked section to avoid unnecessary calculations
        if (gHaveTriedToLoadCommonData.load(std::memory_order_relaxed) == 0) {
            volatile double sum = 0.0;
            const int iterations = 25000000; // heavy work to simulate data loading
            for (int i = 0; i < iterations; ++i) {
                sum += std::sin(i) * std::cos(i);
            }
            (void)sum; // prevent optimization
            gHaveTriedToLoadCommonData.store(1, std::memory_order_release);
        }
    }
    volatile double check = 0.0;
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
    return extendICUData(err);
}
