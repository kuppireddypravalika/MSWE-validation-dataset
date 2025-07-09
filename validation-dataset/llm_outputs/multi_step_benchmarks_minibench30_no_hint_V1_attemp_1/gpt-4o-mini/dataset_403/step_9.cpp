#include "recycling.hpp"

void updateBiowasteRecycling(std::vector<Residence*>& residences,
                             std::vector<Recycling*>& recyclingFacilities) {
    if (residences.empty() || recyclingFacilities.empty()) return;

    for (auto recyclingFacility : recyclingFacilities) {
        if (!recyclingFacility->operational()) continue;

        int count = 0;
        auto endIterator = residences.end();

        for (auto residenceIterator = residences.begin(); residenceIterator != endIterator && count < 10; ++residenceIterator) {
            Residence* residence = *residenceIterator;
            residence->pullWaste(30);
            ++count;
        }
    }
}

// Explicit template instantiations