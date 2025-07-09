#include "common.h"

double information_theoretic_bits(const field_set& fs) {
    double res = 0.0;
    for (const auto& d : fs.disc_and_bits())
        res += custom_log2(d.multy); // uses slow integer specialization
    for (const auto& c : fs.contin()) {
        unsigned contin_count = pow2(c.depth + 1) - 1;
        res += custom_log2(contin_count);
    }
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
    std::cout << information_theoretic_bits_original(fs) << std::endl;
    return 0;
}
#endif
