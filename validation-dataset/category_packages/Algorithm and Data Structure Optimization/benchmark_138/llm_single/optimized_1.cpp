#include "common.h"

double information_theoretic_bits(const field_set& fs) {
    double res = 0.0;
    res = std::accumulate(fs.disc_and_bits().begin(), fs.disc_and_bits().end(), res, [](double sum, const field_set::disc_spec& d) { return sum + custom_log2(d.multy); });
    res += std::accumulate(fs.contin().begin(), fs.contin().end(), 0.0, [](double sum, const field_set::contin_spec& c) { return sum + custom_log2(pow2(c.depth + 1) - 1); });
    res += std::accumulate(fs.onto().begin(), fs.onto().end(), 0.0, [](double sum, const field_set::onto_spec& o) { return sum + custom_log2(o.branching) * static_cast<double>(o.depth); });
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
