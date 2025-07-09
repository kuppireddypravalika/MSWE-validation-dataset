#include <cstddef>
#include <cstdint>

// Original implementation: copies byte by byte
__attribute__((noinline))
void* my_memcpy(void* s1, const void* s2, size_t n) {
    volatile char* dest = static_cast<volatile char*>(s1);
    volatile const char* src = static_cast<volatile const char*>(s2);
    while (n--) {
        *dest++ = *src++;
    }
    return s1;
}
