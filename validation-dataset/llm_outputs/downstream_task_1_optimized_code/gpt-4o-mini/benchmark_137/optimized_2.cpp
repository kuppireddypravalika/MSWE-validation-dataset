#include <string>
#include <memory>

static const std::string large(1024 * 1024, 'x');

unsigned long long process(std::size_t iterations) {
    const bool writable = false;
    const bool asan = true; // simulate -fsanitize=address

    unsigned long long sum = 0;
    for (std::size_t i = 0; i < iterations; ++i) {
        // Allocate once outside the loop to avoid repeated memory allocation
        const std::string *p = &large;
        sum += p->size();  
    }
    return sum;
}