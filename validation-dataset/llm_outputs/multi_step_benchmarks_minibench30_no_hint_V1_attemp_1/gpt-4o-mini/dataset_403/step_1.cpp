#include "recycling.hpp"

void updateBiowasteRecycling(std::vector<Residence*>& residences,
                             std::vector<Recycling*>& recyclingFacilities) {
    if (residences.empty() || recyclingFacilities.empty()) return;

    for (auto recyclingFacility : recyclingFacilities) {
        // Since operational check is trivial, we can early continue if not operational
        if (!recyclingFacility->operational()) continue;

        int count = 0;
        for (auto residenceIterator = residences.begin(); residenceIterator != residences.end() && count < 10; ++residenceIterator) {
            Residence* residence = static_cast<Residence*>(*residenceIterator);
            residence->pullWaste(30);
            ++count;
        }
    }
}


// Explicit template instantiations
