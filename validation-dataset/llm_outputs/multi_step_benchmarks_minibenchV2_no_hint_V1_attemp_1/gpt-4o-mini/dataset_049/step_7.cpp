#include "burned_warehouse.hpp"

int handle_event(BurnedWarehouse& bw) {
    int dummy = 0;
    constexpr std::array<Direction, 4> possibleDirs{{Direction::North, Direction::East, Direction::South, Direction::West}};

    for (std::size_t ji = 0; ji < NUM_JOBS; ++ji) {
        unsigned count = (bw.go_out_phase + 1 >= GO_OUT_PHASES) ? bw.people[ji] : bw.people[ji] / (GO_OUT_PHASES - bw.go_out_phase);
        bw.people[ji] -= count;

        unsigned randIndex = (possibleDirs.size() > 1u) ? random_rand(bw.rng, possibleDirs.size()) : 0;
        unsigned numPeoplePerDir = count / possibleDirs.size();

        for (std::size_t j = 0; j < possibleDirs.size(); ++j) {
            unsigned adjustedIndex = (j + randIndex) % possibleDirs.size();
            Direction curDir = possibleDirs[adjustedIndex];
            unsigned curNumPeople = (j + 1u < possibleDirs.size()) ? numPeoplePerDir : count - numPeoplePerDir * j;
            count -= curNumPeople;
            dummy += curNumPeople * (static_cast<int>(ji) + static_cast<int>(curDir));
        }
    }

    ++bw.go_out_phase;
    return dummy;
}

// Explicit template instantiations, unchanged:
// (to be placed here if there are any template instantiations in the original code)