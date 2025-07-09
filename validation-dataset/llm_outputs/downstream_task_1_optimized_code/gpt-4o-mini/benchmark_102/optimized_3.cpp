#include <cstdint>
#include <cstddef>

// Copy parameters onto a large fixed stack frame then compute a sum.
int copy_params_sum(const int* params, int count) {
    char frame[4096];                // mimic page-sized allocation
    for (size_t i = 0; i < sizeof(frame); ++i) // touch full page
        frame[i] = 0;
    int* sp = reinterpret_cast<int*>(frame + sizeof(frame));
    // no explicit alignment in the original stub
    int sum = 0; 
    for (int i = 0; i < count; ++i) {
        *--sp = params[i];                    // push parameter
        sum += params[i];   // directly compute sum while copying
    }
    return sum;
}