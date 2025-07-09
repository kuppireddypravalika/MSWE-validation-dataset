#include <cstring>
#include <string>
#include <cstddef>

static const char *LHS =
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
static const char *RHS =
"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";

static char *concat_opt(const char *a, const char *b) {
    size_t a_len = strlen(a);
    size_t b_len = strlen(b);
    char *r = new char[a_len + b_len + 1]; // Preallocate required memory
    memcpy(r, a, a_len);
    memcpy(r + a_len, b, b_len + 1); // Copy b and null terminator
    return r;
}

size_t run(size_t loops) {
    size_t sum = 0;
    for (size_t i = 0; i < loops; ++i) {
        char *r = concat_opt(LHS, RHS);
        sum += r[0];
        delete[] r;
    }
    return sum;
}

// Explicit template instantiations should be here if any
