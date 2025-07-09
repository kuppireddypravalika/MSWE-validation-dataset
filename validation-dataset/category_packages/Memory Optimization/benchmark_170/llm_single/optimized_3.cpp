#include <string>
#include <vector>
#include <stdexcept>
#include <numeric>

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
    if (!e.valid) {
        throw std::runtime_error(ctx + ": " + e.err);
    }
}

std::string createSymbol_optimized(const std::string &table, size_t index) {
    const ExpectedStr e = getName(table, index);
    check(e, "objfile");
    return e.value;
}

size_t process(const std::string &table,
               const std::vector<size_t> &indices,
               int repeat) {
    size_t total = 0;
    total.reserve(repeat * indices.size()); // Reserve size for the expected output
    for (int r = 0; r < repeat; ++r) {
        for (size_t idx : indices) {
            total += createSymbol_optimized(table, idx).size();
        }
    }
    return total;
}

// Explicit template instantiation (if any were needed in your original code)
// template class YourTemplate<int, double>; // Example, adjust as necessary
