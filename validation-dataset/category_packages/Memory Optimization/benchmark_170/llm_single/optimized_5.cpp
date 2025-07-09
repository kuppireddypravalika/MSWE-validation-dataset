#include <string>
#include <vector>
#include <stdexcept>

struct ExpectedStr {
    bool valid;
    std::string value;
    std::string err;
};

ExpectedStr getName(const std::string &table, size_t index) {
    if (index >= table.size())
        return {false, "", "invalid index"};
    size_t end = index;
    while (end < table.size() && table[end] != '\0')
        ++end;
    return {true, table.substr(index, end - index), ""};
}

static void check(const ExpectedStr &e, const std::string &ctx) {
    if (!e.valid) {
        throw std::runtime_error(ctx + ": " + e.err);
    }
}

std::string createSymbol(const std::string &table, size_t index) {
    ExpectedStr e = getName(table, index);
    check(e, "objfile");
    return e.value;
}

size_t process(const std::string &table,
               const std::vector<size_t> &indices,
               int repeat) {
    size_t total = 0;
    total.reserve(indices.size() * repeat); // Preallocate total string size for performance
    for (int r = 0; r < repeat; ++r) {
        for (size_t idx : indices) {
            total += createSymbol(table, idx).size();
        }
    }
    return total;
}

// Explicit template instantiation statements (if any)
