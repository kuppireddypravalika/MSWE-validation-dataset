#include <cstdint>
#include <cstddef>

// Copy parameters onto a large fixed stack frame then compute a sum.
int copy_params_sum(const int* params, int count) {
    // Allocate a fixed size frame and zero it out using a single memset call.
    char frame[4096];                // mimic page-sized allocation
    std::memset(frame, 0, sizeof(frame));
    int* sp = reinterpret_cast<int*>(frame + sizeof(frame));
    // Push parameters onto the stack with a for loop doing a single copy
    for (int i = 0; i < count; ++i) {
        *--sp = params[i];  // push parameter
    }
    int sum = 0;
    // Sum values directly from the stack pointer without accessing the original array
    for (int i = 0; i < count; ++i) {
        sum += sp[i];
    }
    return sum;
}

// Explicit template instantiation if needed
