#include "common.h"

double information_theoretic_bits(const field_set& fs) {
    double res = 0.0;
    double log2_2 = std::log(2.0);

    // Optimize for disc specifications
to_iterator disc_end = fs.disc_and_bits().end();
    for (auto it = fs.disc_and_bits().begin(); it != disc_end; ++it) {
        res += std::log(static_cast<double>(it->multy)) / log2_2;
    }

    // Optimize for continuous specifications
    to_iterator contin_end = fs.contin().end();
    for (auto it = fs.contin().begin(); it != contin_end; ++it) {
        unsigned contin_count = pow2(it->depth + 1) - 1;
        res += std::log(static_cast<double>(contin_count)) / log2_2;
    }

    // Optimize for ontology specifications
    to_iterator onto_end = fs.onto().end();
    for (auto it = fs.onto().begin(); it != onto_end; ++it) {
        res += std::log(static_cast<double>(it->branching)) / log2_2 * static_cast<double>(it->depth);
    }

    return res;
}

#ifdef BUILD_MAIN
#include <iostream>
int main() {
    field_set fs;
    fs.discs.push_back({16});
    fs.contins.push_back({4});
    fs.ontos.push_back({8,2});
    std::cout << information_theoretic_bits(fs) << std::endl;
    return 0;
}
#endif
