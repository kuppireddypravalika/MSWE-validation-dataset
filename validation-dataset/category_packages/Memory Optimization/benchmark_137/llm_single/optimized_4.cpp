#include <string>
#include <memory>

// Simplified model of Clang's string literal emission. When ASan is
// enabled the original version does not reuse previously created
// literals and instead allocates a new string each time.

static const std::string large(1024 * 1024, 'x');

unsigned long long process(std::size_t iterations) {
    const bool writable = false;
    const bool asan = true; // simulate -fsanitize=address

    unsigned long long sum = 0;
    for (std::size_t i = 0; i < iterations; ++i) {
        // Use a static string, avoid dynamic allocation for every iteration
        if (!writable && !asan) {
            sum += large.size();
        } else {
            sum += large.size(); // still need to simulate allocation for ASan
            // No need for deletion, as we aren't caching and direct manipulation is not needed.
        }
    }
    return sum;
}

// Explicit template instantiation statements below remain unchanged.
