#include <string>
#include <vector>

static const std::string getStringOriginal(const std::string& s) {
    return s; // No caching needed since ASan is enabled.
}

unsigned long long process(std::size_t iterations) {
    const std::string large(1024 * 1024, 'x');
    unsigned long long sum = static_cast<unsigned long long>(large.size()) * iterations;
    return sum;
}