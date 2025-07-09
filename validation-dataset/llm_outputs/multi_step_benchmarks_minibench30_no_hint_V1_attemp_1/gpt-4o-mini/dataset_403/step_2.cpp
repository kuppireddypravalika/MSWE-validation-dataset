#include "recycling.hpp"

void updateBiowasteRecycling(std::vector<Residence*>& residences,
                             std::vector<Recycling*>& recyclingFacilities) {
    if (residences.empty() || recyclingFacilities.empty()) return;

    for (auto recyclingFacility : recyclingFacilities) {
        // Early continue if not operational
        if (!recyclingFacility->operational()) continue;

        int count = 0;
        auto residenceIterator = residences.begin();
        for (; residenceIterator != residences.end() && count < 10; ++residenceIterator) {
            Residence* residence = *residenceIterator;
            residence->pullWaste(30);
            ++count;
        }
    }
}

// Explicit template instantiations