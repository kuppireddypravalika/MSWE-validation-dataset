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
    for (size_t i = 0; i < count; ++i) {
        while (isspace((unsigned char)*ptr)) ++ptr;
        char* end;
        double v = mystrtod(ptr, &end);
        sum += v;
        ptr = end;
    }
    return sum;
}
