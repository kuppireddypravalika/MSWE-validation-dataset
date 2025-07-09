#include <string>
#include <memory>

static const std::string* getStringOriginal(const std::string& s, bool writable, bool asan) {
    return new std::string(s); // always allocate a new string with ASan or writable
}

unsigned long long process(std::size_t iterations) {
    const std::string large(1024 * 1024, 'x');
    unsigned long long sum = iterations * large.size(); // calculate total size
    for (std::size_t i = 0; i < iterations; ++i) {
        const std::string* p = getStringOriginal(large, false, true);
        delete p;
    }
    return sum;
}

// Explicit template instantiation statements (if any) from the original code should be reproduced here unchanged.