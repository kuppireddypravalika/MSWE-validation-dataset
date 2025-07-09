#include <cstddef>

void initgdt(volatile int* dest, const int* src1, const int* src2,
                       std::size_t size, int repeat) {
    for (int r = 0; r < repeat; ++r) {
        for (std::size_t i = 0; i < size; ++i) {
            dest[i] = src1[i] + src2[i];
        }
    }
}
