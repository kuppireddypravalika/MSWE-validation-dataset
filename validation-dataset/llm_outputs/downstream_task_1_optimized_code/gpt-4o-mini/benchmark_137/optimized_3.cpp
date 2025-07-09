#include <string>
#include <memory>

static std::string cached_original; // Use a single static string for caching

static const std::string* getStringOptimized(const std::string& s, bool writable, bool asan) {
    if (!writable && !asan) {
        return &cached_original; // Return the cached string directly
    }
    return new std::string(s); // no caching with ASan or writable strings
}

unsigned long long process(std::size_t iterations) {
    const bool writable = false;
    const bool asan = true; // simulate -fsanitize=address
    const std::string large(1024 * 1024, 'x');

    unsigned long long sum = 0;
    for (std::size_t i = 0; i < iterations; ++i) {
        const std::string* p = getStringOptimized(large, writable, asan);
        sum += p->size();
        // With ASan enabled the string is newly allocated each time.
        if (asan) {
            delete p; // Ensure to delete only when allocated
        }
    }
    return sum;
}

// Explicit template instantiation statements go here, if any.