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

std::string createSymbol_optimized(const std::string &table, size_t index) {
    ExpectedStr nameOrErr = getName(table, index);
    if (!nameOrErr.valid)
        throw std::runtime_error(std::string("objfile: ") + nameOrErr.err);
    return nameOrErr.value;
}

size_t process(const std::string &table,
                         const std::vector<size_t> &indices,
                         int repeat) {
    size_t total = 0;
    for (int r = 0; r < repeat; ++r)
        for (size_t idx : indices)
            total += createSymbol_optimized(table, idx).size();
    return total;
}
