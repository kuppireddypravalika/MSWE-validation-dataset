#ifndef BENCHMARK_138_COMMON_H
#define BENCHMARK_138_COMMON_H

#include <vector>
#include <cmath>

struct field_set {
    struct disc_spec { unsigned multy; };
    struct contin_spec { unsigned depth; };
    struct onto_spec { unsigned branching; unsigned depth; };

    std::vector<disc_spec> discs;
    std::vector<contin_spec> contins;
    std::vector<onto_spec> ontos;

    const std::vector<disc_spec>& disc_and_bits() const { return discs; }
    const std::vector<contin_spec>& contin() const { return contins; }
    const std::vector<onto_spec>& onto() const { return ontos; }
};

inline unsigned pow2(unsigned x) { return 1u << x; }

// Generic fast floating point log2
template <typename T>
inline double custom_log2(T value) {
    return std::log(static_cast<double>(value)) / std::log(2.0);
}

// Slow integer specialization that wastes CPU cycles but yields the same result
template <>
inline double custom_log2<unsigned>(unsigned value) {
    double result = std::log(static_cast<double>(value)) / std::log(2.0);
    volatile double sink = 0.0;
    for (int i = 0; i < 1000; ++i) {
        sink += result;
    }
    if (sink > 1e9)
        result += sink; // never taken, prevents optimization
    return result;
}

#endif // BENCHMARK_138_COMMON_H
