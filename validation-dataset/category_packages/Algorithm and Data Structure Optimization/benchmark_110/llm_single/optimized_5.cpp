#include <cstddef>

void initgdt(volatile int* dest, const int* src1, const int* src2, std::size_t size, int repeat) {
    // Unrolling the inner loop for better performance
    for (int r = 0; r < repeat; ++r) {
        std::size_t i = 0;
        // Process 4 elements at a time if size is large enough
        for (; i + 4 <= size; i += 4) {
            dest[i] = src1[i];
            dest[i + 1] = src1[i + 1];
            dest[i + 2] = src1[i + 2];
            dest[i + 3] = src1[i + 3];
        }
        // Handle any remaining elements
        for (; i < size; ++i) {
            dest[i] = src1[i];
        }

        i = 0;
        // Again, process 4 elements if possible
        for (; i + 4 <= size; i += 4) {
            dest[i] += src2[i];
            dest[i + 1] += src2[i + 1];
            dest[i + 2] += src2[i + 2];
            dest[i + 3] += src2[i + 3];
        }
        // Handle any remaining elements
        for (; i < size; ++i) {
            dest[i] += src2[i];
        }
    }
}
