#include <string>
#include <memory>

// Optimized variant: string literals are merged even with ASan enabled,
// so the allocation happens only once and subsequent calls reuse it.

static std::string* cached_optimized = nullptr;

static const std::string* getStringOptimized(const std::string& s,
                                             bool writable,
                                             bool /*asan*/) {
    if (!writable) {
        if (!cached_optimized)
            cached_optimized = new std::string(s);
        return cached_optimized;
    }
    return new std::string(s);
}

unsigned long long process(std::size_t iterations) {
    const bool writable = false;
    const bool asan = true; // still "enabled" but no longer disables caching
    const std::string large(1024 * 1024, 'x');

    unsigned long long sum = 0;
    for (std::size_t i = 0; i < iterations; ++i) {
        const std::string* p = getStringOptimized(large, writable, asan);
        sum += p->size();
        // Only allocate once, nothing to delete inside the loop.
    }
    return sum;
}
