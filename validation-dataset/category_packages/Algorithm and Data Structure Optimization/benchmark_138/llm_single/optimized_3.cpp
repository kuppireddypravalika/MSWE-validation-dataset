#include "common.h"

constexpr double FAST_LOG2_BASE = 1.4426950408889634073599246811823; // 1/log(2)

double information_theoretic_bits(const field_set& fs) {
    double res = 0.0;
    // Use precomputed fast log2 calculations directly
    for (const auto& d : fs.disc_and_bits())
        res += std::log(static_cast<double>(d.multy)) * FAST_LOG2_BASE;
    for (const auto& c : fs.contin()) {
        unsigned contin_count = (1u << (c.depth + 1)) - 1; // use shift for pow2
        res += std::log(static_cast<double>(contin_count)) * FAST_LOG2_BASE;
    }
    for (const auto& o : fs.onto())
        res += std::log(static_cast<double>(o.branching)) * o.depth * FAST_LOG2_BASE;
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
