#include <cstring>
#include <string>
#include <cstddef>

static const char *LHS =
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
static const char *RHS =
"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";

static char* concat_optimized(const char* a, const char* b) {
    // Calculate combined size
    size_t lenA = std::strlen(a);
    size_t lenB = std::strlen(b);
    char* r = new char[lenA + lenB + 1];
    // Use memcpy for direct copy without extra heap allocation
    std::memcpy(r, a, lenA);
    std::memcpy(r + lenA, b, lenB + 1); // +1 to copy null terminator
    return r;
}

size_t run(size_t loops) {
    size_t sum = 0;
    for (size_t i = 0; i < loops; ++i) {
        char *r = concat_optimized(LHS, RHS);
        sum += r[0];
        delete[] r;
    }
    return sum;
}
