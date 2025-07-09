#include "shared.hpp"

bool RifColumnBasedUserData::values(const int& resultAddress, std::vector<double>* values) const {
    auto search = m_map.find(resultAddress);
    if (search != m_map.end()) {
        auto tableColIndices = search->second;
        const ColumnInfo* ci = m_parser->columnInfo(tableColIndices.first, tableColIndices.second);
        if (!ci) return false;

        // Reserve memory for values to avoid repeated allocations
        values->clear(); // Clear existing values if any
        values->reserve(ci->values.size()); // Optimize for push_back
        values->insert(values->end(), ci->values.begin(), ci->values.end()); // Efficient copying
    }
    return true;
}