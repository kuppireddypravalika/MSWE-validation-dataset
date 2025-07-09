#include <cstring>
#include <string>
#include <cstddef>

static const char *LHS =
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
static const char *RHS =
"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";

size_t run(size_t loops) {
    size_t sum = 0;
    const size_t lhs_len = strlen(LHS);
    const size_t rhs_len = strlen(RHS);
    const size_t total_size = lhs_len + rhs_len + 1;

    // Allocate a single buffer once outside the loop
    char *r = new char[total_size];

    for (size_t i = 0; i < loops; ++i) {
        memcpy(r, LHS, lhs_len);
        memcpy(r + lhs_len, RHS, rhs_len + 1); // +1 to copy the null terminator
        sum += r[0];
    }

    delete[] r; // Free memory after the loop
    return sum;
}

// Explicit template instantiations, if any, remain unchanged
