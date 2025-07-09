#include "recycling.hpp"

void updateBiowasteRecycling(std::vector<Residence*>& residences,
                             std::vector<Recycling*>& recyclingFacilities) {
    if (residences.empty() || recyclingFacilities.empty()) return;

    auto residenceIterator = residences.begin();
    for (auto recyclingFacility : recyclingFacilities) {
        int count = 0;

        // Consider a different control path
        if (!recyclingFacility->operational()) { continue; }

        for (; residenceIterator != residences.end(); ++residenceIterator) {
            Residence* residence = static_cast<Residence*>(*residenceIterator);
            residence->pullWaste(30);
            ++count;
            if (count >= 10) { break; }
        }
    }
}
