#include "common.h"

double information_theoretic_bits(const field_set& fs) {
    double res = 0.0;
    // Reserve space for potential memory improvements.
    const auto& discs = fs.disc_and_bits();
    const auto& contins = fs.contin();
    const auto& ontos = fs.onto();
    
    // Combine loops and avoid repeated function calls
    for (const auto& d : discs) {
        res += custom_log2(d.multy);  // still uses slow integer specialization
    }
    
    for (const auto& c : contins) {
        res += custom_log2((1u << (c.depth + 1)) - 1);  // Replace pow2 call
    }
    
    for (const auto& o : ontos) {
        res += custom_log2(o.branching) * static_cast<double>(o.depth);
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