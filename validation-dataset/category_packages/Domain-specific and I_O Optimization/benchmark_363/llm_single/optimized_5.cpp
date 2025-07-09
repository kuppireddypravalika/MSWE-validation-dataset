#include <cctype>
#include <cstdlib>
#include <cmath>
#include <cstddef>

static inline double mystrtod(const char* nptr, char** endptr) {
    return strtod(nptr, endptr);
}

double parse_numbers(const char* text, size_t count) {
    const char* ptr = text;
    double sum = 0.0;
    for (size_t i = 0; i < count; ++i) {
        while (isspace(static_cast<unsigned char>(*ptr))) {
            ++ptr;
        }
        if (*ptr == '\0') break;  // Handle end of string
        char* end;
        double v = mystrtod(ptr, &end);
        sum += v;
        ptr = end;
    }
    return sum;
}

// Explicit template instantiation statements
