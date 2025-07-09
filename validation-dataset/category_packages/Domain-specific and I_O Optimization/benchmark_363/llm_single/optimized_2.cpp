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
    while (count > 0) {
        // Skip leading whitespace
        while (isspace(static_cast<unsigned char>(*ptr))) ++ptr;
        if (*ptr == '\0') break; // Handle case where text ends
        char* end;
        double v = mystrtod(ptr, &end);
        sum += v;
        ptr = end;
        --count;
    }
    return sum;
}

// Explicit template instantiation statements (if needed) go here.
