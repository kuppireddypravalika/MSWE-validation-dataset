#include "recycling.hpp"

void updateBiowasteRecycling(std::vector<Residence*>& residences,
                             std::vector<Recycling*>& recyclingFacilities) {
    if (residences.empty() || recyclingFacilities.empty()) return;

    auto residenceIterator = residences.begin();
    for (auto recyclingFacility : recyclingFacilities) {
        int count = 0;

        if (!recyclingFacility->operational()) { continue; } // Consider a different control structure

        Recycling* recycling = static_cast<Recycling*>(recyclingFacility);

        for (; residenceIterator != residences.end(); ++residenceIterator) {
            Residence* residence = static_cast<Residence*>(*residenceIterator);
            residence->pullWaste(recycling->wasteProcessingCapacity());

            ++count;

            if (count >= recycling->residentialSupportCount()) {
                break;
            }
        }

        // We got to the end of the residence list, don't waste time iterating over
        // any remaining recycling facilities.
        if (residenceIterator == residences.end()) { break; }
    }
}
