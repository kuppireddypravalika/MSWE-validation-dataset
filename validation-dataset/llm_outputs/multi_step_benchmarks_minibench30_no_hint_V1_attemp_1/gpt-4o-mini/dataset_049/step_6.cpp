#include "burned_warehouse.hpp"

int handle_event(BurnedWarehouse& bw) {
    int dummy = 0;
    constexpr std::array<Direction, 4> possibleDirs = {Direction::North, Direction::East, Direction::South, Direction::West};

    for (std::size_t ji = 0; ji < NUM_JOBS; ++ji) {
        const unsigned count = (bw.go_out_phase + 1u >= GO_OUT_PHASES) ? bw.people[ji] : bw.people[ji] / (GO_OUT_PHASES - bw.go_out_phase);
        bw.people[ji] -= count;

        unsigned randomDirIndex = (possibleDirs.size() > 1) ? random_rand(bw.rng, possibleDirs.size()) : 0;
        unsigned numPeoplePerDir = count / possibleDirs.size();
        unsigned remainingPeople = count;

        for (std::size_t j = 0; j < possibleDirs.size(); ++j) {
            unsigned idx = (j + randomDirIndex) % possibleDirs.size();
            Direction curDir = possibleDirs[idx];
            unsigned curNumPeople = (j + 1u < possibleDirs.size()) ? numPeoplePerDir : remainingPeople;
            remainingPeople -= curNumPeople;

            dummy += curNumPeople * (static_cast<int>(ji) + static_cast<int>(curDir));
        }
    }

    ++bw.go_out_phase;
    return dummy;
}

// Explicit template instantiations if necessary (unchanged from original)