#include "shared.hpp"

bool RifColumnBasedUserData::values(const int& resultAddress, std::vector<double>* values) const {
    auto search = m_map.find(resultAddress);
    if (search != m_map.end()) {
        auto tableColIndices = search->second;
        const ColumnInfo* ci = m_parser->columnInfo(tableColIndices.first, tableColIndices.second);
        if (!ci) return false;

        for (const auto& v : ci->values) {
            values->push_back(v);
        }
    }
    return true;
}

// explicit instantiation helper if needed
