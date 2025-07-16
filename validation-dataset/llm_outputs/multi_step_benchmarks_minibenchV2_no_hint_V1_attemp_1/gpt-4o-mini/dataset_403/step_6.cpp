#include "recycling.hpp"

void updateBiowasteRecycling(std::vector<Residence*>& residences, std::vector<Recycling*>& recyclingFacilities) {
    if (residences.empty() || recyclingFacilities.empty()) return;

    // Pre-calculate the limit constant
    const int MAX_PROCESS = 10;

    for (Recycling* recyclingFacility : recyclingFacilities) {
        if (!recyclingFacility->operational()) continue;

        // Processing up to 10 residences for each operational recycling facility
        int processedCount = 0;
        for (Residence* residence : residences) {
            residence->pullWaste(30);
            if (++processedCount >= MAX_PROCESS) break;
        }
    }
}

// Explicit template instantiations (if any) follow here.