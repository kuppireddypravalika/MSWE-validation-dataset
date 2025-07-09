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
    size_t end = table.find('\0', index);
    if (end == std::string::npos) end = table.size();
    return {true, table.substr(index, end - index), ""};
}

static void check(const ExpectedStr &e, const std::string &ctx) {
    // Check validity and potentially throw error without string allocation
    if (!e.valid) {
        throw std::runtime_error(ctx + ": " + e.err);
    }
}

std::string createSymbol_optimized(const std::string &table, size_t index) {
    ExpectedStr name = getName(table, index);
    check(name, "objfile");
    return name.value;
}

size_t process(const std::string &table,
               const std::vector<size_t> &indices,
               int repeat) {
    size_t total = 0;
    total.reserve(repeat * indices.size());
    for (int r = 0; r < repeat; ++r) {
        for (size_t idx : indices) {
            total += createSymbol_optimized(table, idx).size();
        }
    }
    return total;
}

// Explicit template instantiation statements retained below
// template instantiations if any
