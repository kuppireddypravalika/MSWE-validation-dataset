#include "recycling.hpp"

void updateBiowasteRecycling(std::vector<Residence*>& residences,
                             std::vector<Recycling*>& recyclingFacilities) {
    if (residences.empty() || recyclingFacilities.empty()) return;

    for (auto recyclingFacility : recyclingFacilities) {
        // Check if the recycling facility is operational and reserve iterators only when necessary
        if (!recyclingFacility->operational()) { continue; }

        int count = 0;
        // Directly access elements instead of using an iterator
        for (size_t i = 0; i < residences.size() && count < 10; ++i) {
            Residence* residence = residences[i]; // Avoid using iterator to access residence
            residence->pullWaste(30);
            ++count;
        }
    }
}


// Explicit template instantiations (if any) follow here.