#include "recycling.hpp"

void updateBiowasteRecycling(std::vector<Residence*>& residences,
                             std::vector<Recycling*>& recyclingFacilities) {
    if (residences.empty() || recyclingFacilities.empty()) return;

    const int MAX_PROCESS = 10;
    for (Recycling* recyclingFacility : recyclingFacilities) {
        if (!recyclingFacility->operational()) continue;  // Skip non-operational facilities

        int processedCount = 0;
        for (Residence* residence : residences) {
            residence->pullWaste(30);
            if (++processedCount >= MAX_PROCESS) break;  // Stop processing after 10 residences
        }
    }
}

// Explicit template instantiations (if any) follow here.