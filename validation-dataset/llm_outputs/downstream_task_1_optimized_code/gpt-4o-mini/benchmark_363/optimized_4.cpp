#include <cctype>
#include <cstdlib>
#include <cmath>
#include <cstddef>

// Use standard strtod
static inline double mystrtod(const char* nptr, char** endptr) {
    return strtod(nptr, endptr);
}

double parse_numbers(const char* text, size_t count) {
    const char* ptr = text;
    double sum = 0.0;
    const char* end_ptr = text + count;

    while (ptr < end_ptr) {
        // Skip whitespace
        while (ptr < end_ptr && isspace((unsigned char)*ptr)) {
            ++ptr;
        }
        if (ptr >= end_ptr) break;  // Break if we reach the end
        char* end;
        double v = mystrtod(ptr, &end);
        sum += v;
        ptr = end;
    }
    return sum;
}