#ifndef RIF_COLUMN_BASED_USER_DATA_HPP
#define RIF_COLUMN_BASED_USER_DATA_HPP
#include <vector>
#include <unordered_map>
#include <memory>
#include <cstddef>

struct ColumnInfo {
    std::vector<double> values;
};

class Parser {
public:
    std::vector<std::vector<ColumnInfo>> cols;
    Parser(std::size_t num_values) {
        cols.resize(1);
        cols[0].resize(1);
        cols[0][0].values.reserve(num_values);
        for (std::size_t i = 0; i < num_values; ++i) {
            cols[0][0].values.push_back(static_cast<double>(i));
        }
    }
    const ColumnInfo* columnInfo(std::size_t table, std::size_t col) const {
        if (table >= cols.size() || col >= cols[table].size()) return nullptr;
        return &cols[table][col];
    }
};

class RifColumnBasedUserData {
    std::unordered_map<int, std::pair<std::size_t,std::size_t>> m_map;
    std::unique_ptr<Parser> m_parser;
public:
    explicit RifColumnBasedUserData(std::size_t num_values)
        : m_parser(std::make_unique<Parser>(num_values)) {
        m_map[0] = {0,0};
    }

    bool values(const int& resultAddress, std::vector<double>* values) const;
};

#endif
