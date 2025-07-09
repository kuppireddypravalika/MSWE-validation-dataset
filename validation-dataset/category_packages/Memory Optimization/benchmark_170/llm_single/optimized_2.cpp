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
    const ExpectedStr e = getName(table, index);
    check(e, "objfile");
    return e.value;
}

size_t process(const std::string &table,
               const std::vector<size_t> &indices,
               int repeat) {
    size_t total = 0;
    total = std::accumulate(indices.begin(), indices.end(), total,
                  [&](size_t acc, size_t idx) {
                      return acc + createSymbol(table, idx).size() * repeat;
                  });
    return total;
}

// Explicit template instantiations from the original code
