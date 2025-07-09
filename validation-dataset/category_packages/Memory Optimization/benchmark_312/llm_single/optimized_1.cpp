#include <cstring>
#include <string>
#include <cstddef>

static const char *LHS =
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
static const char *RHS =
"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";

static char *concat_opt(const char *a, const char *b) {
    size_t len_a = std::strlen(a);
    size_t len_b = std::strlen(b);
    char *r = new char[len_a + len_b + 1];
    std::memcpy(r, a, len_a);
    std::memcpy(r + len_a, b, len_b + 1); // include null terminator
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

// Explicit template instantiation statements if any go here
