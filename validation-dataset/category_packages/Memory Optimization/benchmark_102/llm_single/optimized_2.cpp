#include <cstdint>
#include <cstddef>

// Copy parameters onto a large fixed stack frame then compute a sum.
int copy_params_sum(const int* params, int count) {
    // Declare variables
    int sum = 0;
    
    // Loop through the parameters and compute the sum directly
    for (int i = 0; i < count; ++i) {
        sum += params[i];
    }
    
    return sum;
}
