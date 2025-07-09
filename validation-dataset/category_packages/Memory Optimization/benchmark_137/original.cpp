#include <string>
#include <memory>

// Simplified model of Clang's string literal emission. When ASan is
// enabled the original version does not reuse previously created
// literals and instead allocates a new string each time.

static std::string* cached_original = nullptr;

static const std::string* getStringOriginal(const std::string& s,
                                            bool writable,
                                            bool asan) {
    if (!writable && !asan) {
        if (!cached_original)
            cached_original = new std::string(s);
        return cached_original;
    }
    return new std::string(s); // no caching with ASan or writable strings
}

unsigned long long process(std::size_t iterations) {
    const bool writable = false;
    const bool asan = true; // simulate -fsanitize=address
    const std::string large(1024 * 1024, 'x');

    unsigned long long sum = 0;
    for (std::size_t i = 0; i < iterations; ++i) {
        const std::string* p = getStringOriginal(large, writable, asan);
        sum += p->size();
        // With ASan enabled the string is newly allocated each time.
        delete p;
    }
    return sum;
}
