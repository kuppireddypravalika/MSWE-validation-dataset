#include "common.h"

double information_theoretic_bits(const field_set& fs) {
    double res = 0.0;

    // Precompute log2 values for discrete specs
    for (const auto& d : fs.disc_and_bits())
        res += custom_log2(d.multy);

    // Precompute log2 values for continuous specs
    for (const auto& c : fs.contin()) {
        unsigned contin_count = (1u << (c.depth + 1)) - 1; // Use left shift directly
        res += custom_log2(contin_count);
    }

    // Precompute log2 values for ontology specs
    for (const auto& o : fs.onto())
        res += custom_log2(o.branching) * static_cast<double>(o.depth);

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