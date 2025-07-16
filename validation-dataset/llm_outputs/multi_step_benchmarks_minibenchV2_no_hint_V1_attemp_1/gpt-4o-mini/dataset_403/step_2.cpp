#include "recycling.hpp"

void updateBiowasteRecycling(std::vector<Residence*>& residences,
                             std::vector<Recycling*>& recyclingFacilities) {
    if (residences.empty() || recyclingFacilities.empty()) return;

    for (auto* recyclingFacility : recyclingFacilities) {
        // Skip non-operational facilities directly
        if (!recyclingFacility->operational()) { continue; }

        // For operational facilities, process up to 10 residences
        int processedCount = 0;
        for (Residence* residence : residences) {
            residence->pullWaste(30);
            
            // Stop processing after 10 residences
            if (++processedCount >= 10) break;
        }
    }
}

// Explicit template instantiations (if any) follow here.