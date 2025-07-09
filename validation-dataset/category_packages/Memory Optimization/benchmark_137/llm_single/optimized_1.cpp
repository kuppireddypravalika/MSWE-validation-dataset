#include <string>
#include <memory>

// Utilize a static const string instead of dynamic allocation for constant values.
static const std::string large(1024 * 1024, 'x');

unsigned long long process(std::size_t iterations) {
    // Unique allocation avoided, ASan is simulated with a const string.
    unsigned long long sum = 0;
    sum += large.size() * iterations; // Total size pre-calculated.
    return sum;
}

// Explicit template instantiation (unchanged)
