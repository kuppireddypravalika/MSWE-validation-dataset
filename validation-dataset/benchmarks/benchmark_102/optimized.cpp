#include <cstdint>
#include <cstddef>
#include <cstdlib>

// Allocate only as much stack as needed for the parameters and align the
// resulting pointer to 16 bytes before copying. This mirrors the patched
// behaviour in the JDK stub generator.
int copy_params_sum(const int* params, int count) {
    size_t bytes = static_cast<size_t>(count) * sizeof(int);
    char* base = static_cast<char*>(alloca(bytes + 16));
    int* sp = reinterpret_cast<int*>(base + bytes);
    sp = reinterpret_cast<int*>((reinterpret_cast<uintptr_t>(sp) & ~0xF));
    for (int i = 0; i < count; ++i) {
        *--sp = params[i];
    }
    int sum = 0;
    for (int i = 0; i < count; ++i)
        sum += sp[i];
    return sum;
}
