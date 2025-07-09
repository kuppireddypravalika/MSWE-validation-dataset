#include <cstddef>

void initgdt(volatile int* dest, const int* src1, const int* src2, std::size_t size, int repeat) {
    for (int r = 0; r < repeat; ++r) {
        std::size_t i = 0;
        // Unroll the loop for better performance
        for (; i + 4 <= size; i += 4) {
            dest[i] = src1[i];
            dest[i + 1] = src1[i + 1];
            dest[i + 2] = src1[i + 2];
            dest[i + 3] = src1[i + 3];
        }
        // Handle the remaining elements
        for (; i < size; ++i) {
            dest[i] = src1[i];
        }
        
        i = 0;
        for (; i + 4 <= size; i += 4) {
            dest[i] += src2[i];
            dest[i + 1] += src2[i + 1];
            dest[i + 2] += src2[i + 2];
            dest[i + 3] += src2[i + 3];
        }
        for (; i < size; ++i) {
            dest[i] += src2[i];
        }
    }
}
