#include <cstring>
#include <string>
#include <cstddef>

static const char *LHS =
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
static const char *RHS =
"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";

static char *concat_opt(const char *a, const char *b) {
    size_t len1 = strlen(a);
    size_t len2 = strlen(b);
    char *r = new char[len1 + len2 + 1];
    memcpy(r, a, len1);
    memcpy(r + len1, b, len2);
    r[len1 + len2] = '\0';
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
