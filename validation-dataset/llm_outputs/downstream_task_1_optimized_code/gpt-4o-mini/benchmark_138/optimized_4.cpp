#include "common.h"


double information_theoretic_bits(const field_set& fs) {
    double res = 0.0;
    const double log2_2 = std::log(2.0);

    // Use reserve to avoid reallocations
    const auto& discs = fs.disc_and_bits();
    for (const auto& d : discs) {
        // Directly calculate log2 for unsigned
        res += std::log(static_cast<double>(d.multy)) / log2_2;
    }

    const auto& contins = fs.contin();
    for (const auto& c : contins) {
        unsigned contin_count = (1u << (c.depth + 1)) - 1; // Use bit shifting instead of pow2
        res += std::log(static_cast<double>(contin_count)) / log2_2;
    }

    const auto& ontos = fs.onto();
    for (const auto& o : ontos) {
        // Avoid redundant conversion to double
        res += std::log(static_cast<double>(o.branching)) / log2_2 * o.depth;
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