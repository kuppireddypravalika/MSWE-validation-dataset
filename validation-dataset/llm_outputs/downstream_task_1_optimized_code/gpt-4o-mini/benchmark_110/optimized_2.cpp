#include <cstddef>

void initgdt(volatile int* dest, const int* src1, const int* src2, std::size_t size, int repeat) {
    if (repeat == 0 || size == 0) return;
    const int* end = src1 + size;
    while (repeat--) {
        int* d = dest;
        const int* s1 = src1;
        const int* s2 = src2;
        while (s1 < end) {
            *d++ = *s1++;
        }
        s1 = src1;
        while (s1 < end) {
            *d++ += *s2++;
        }
    }
}