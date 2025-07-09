#include <cstddef>
#include <cstdint>

// Optimized implementation: copy 8 bytes at a time for aligned pointers
__attribute__((noinline))
void* my_memcpy(void* s1, const void* s2, size_t n) {
    volatile char* dest = static_cast<volatile char*>(s1);
    volatile const char* src = static_cast<volatile const char*>(s2);

    if (((uintptr_t)src & 7) == ((uintptr_t)dest & 7)) {
        while (((uintptr_t)src & 7) && n--) {
            *dest++ = *src++;
        }

        const volatile uint64_t* srcq = reinterpret_cast<const volatile uint64_t*>(src);
        volatile uint64_t* destq = reinterpret_cast<volatile uint64_t*>(dest);
        while (n >= 8) {
            *destq++ = *srcq++;
            n -= 8;
        }

        src = reinterpret_cast<const volatile char*>(srcq);
        dest = reinterpret_cast<volatile char*>(destq);
    }

    while (n--) {
        *dest++ = *src++;
    }
    return s1;
}
