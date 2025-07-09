#include "shared.hpp"

bool RifColumnBasedUserData::values(const int& resultAddress, std::vector<double>* values) const {
    auto search = m_map.find(resultAddress);
    if (search != m_map.end()) {
        auto tableColIndices = search->second;
        const ColumnInfo* ci = m_parser->columnInfo(tableColIndices.first, tableColIndices.second);
        if (!ci) return false;

        // Efficiently copy the values without repeated allocations
        values->clear(); // Clear existing values to avoid accumulation
        values->reserve(ci->values.size()); // Reserve needed space
        std::copy(ci->values.begin(), ci->values.end(), std::back_inserter(*values)); // Use std::copy for optimized performance
    }
    return true;
}

// explicit instantiation helper if needed