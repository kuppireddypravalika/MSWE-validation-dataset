#include <cstring>
#include <string>
#include <cstddef>

static const char *LHS =
"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
static const char *RHS =
"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";

static char *concat_orig(const char *a, const char *b) {
    std::string tmp = std::string(a) + b; // heap allocation for concatenation
    char *r = new char[tmp.size() + 1];
    memcpy(r, tmp.c_str(), tmp.size() + 1);
    return r;
}

size_t run(size_t loops) {
    size_t sum = 0;
    for (size_t i = 0; i < loops; ++i) {
        char *r = concat_orig(LHS, RHS);
        sum += r[0];
        delete[] r;
    }
    return sum;
}
