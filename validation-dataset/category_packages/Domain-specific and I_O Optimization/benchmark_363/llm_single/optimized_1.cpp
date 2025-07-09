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

    // Use a single loop to process all characters
    for (size_t i = 0; i < count; ++i) {
        // Skip leading whitespace
        while (isspace(static_cast<unsigned char>(*ptr))) ++ptr;
        if (*ptr == '\0') break; // Stop if we've hit the end of the string
        char* end;
        double v = mystrtod(ptr, &end);
        sum += v;
        ptr = end;

        // Additional whitespace check after number
        while (isspace(static_cast<unsigned char>(*ptr))) ++ptr;
    }
    return sum;
}

// Explicit template instantiation statements (if any)
