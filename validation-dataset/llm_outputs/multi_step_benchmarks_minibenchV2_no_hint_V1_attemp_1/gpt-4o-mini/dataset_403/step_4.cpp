#include "recycling.hpp"

void updateBiowasteRecycling(std::vector<Residence*>& residences, std::vector<Recycling*>& recyclingFacilities) {
    if (residences.empty() || recyclingFacilities.empty()) return;

    for (Recycling* recyclingFacility : recyclingFacilities) {
        if (!recyclingFacility->operational()) continue; // Skip non-operational facilities

        // Process up to 10 residences for each operational recycling facility
        int processedCount = 0;
        for (auto* residence : residences) {
            residence->pullWaste(30);
            if (++processedCount >= 10) break; // Stop processing after 10 residences
        }
    }
}

// Explicit template instantiations (if any) follow here.