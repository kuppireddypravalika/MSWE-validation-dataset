#include "burned_warehouse.hpp"

int handle_event(BurnedWarehouse& bw) {
    int dummy = 0;
    std::vector<Direction> possibleDirs{Direction::North, Direction::East,
                                        Direction::South, Direction::West};

    for (std::size_t ji = 0; ji < NUM_JOBS; ++ji) {
        unsigned count;
        if (bw.go_out_phase + 1 >= GO_OUT_PHASES)
            count = bw.people[ji];
        else
            count = bw.people[ji] / (GO_OUT_PHASES - bw.go_out_phase);

        bw.people[ji] -= count;

        unsigned startIdx = 0;
        if (possibleDirs.size() > 1u)
            random_rand(bw.rng, possibleDirs.size());
        unsigned numPeoplePerDir = count / possibleDirs.size();

        for (std::size_t j = 0; j < possibleDirs.size(); ++j) {
            const unsigned idx = j + startIdx;
            Direction curDir =
                possibleDirs[idx < possibleDirs.size() ? idx : idx - possibleDirs.size()];
            unsigned curNumPeople =
                (j + 1u < possibleDirs.size()) ? numPeoplePerDir : count;
            count -= curNumPeople;
            for (unsigned z = 0; z < curNumPeople; ++z) {
                dummy += static_cast<int>(ji) + static_cast<int>(curDir);
            }
        }
    }

    ++bw.go_out_phase;
    return dummy;
}
