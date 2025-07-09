#ifndef BITSET_COMMON_H
#define BITSET_COMMON_H

#include <array>
#include <cstdint>
#include <cstddef>
#include <algorithm>
#include <random>

namespace bench238 {

inline size_t hash_combine(size_t seed, size_t value) {
    return seed ^ (value + 0x9e3779b97f4a7c15ull + (seed<<6) + (seed>>2));
}

inline size_t murmur_hash(const uint8_t* data, size_t len) {
    const uint64_t m = 0xc6a4a7935bd1e995ULL;
    const int r = 47;
    uint64_t h = len * m;
    const uint64_t* d = reinterpret_cast<const uint64_t*>(data);
    size_t n = len / 8;
    for(size_t i=0;i<n;++i) {
        uint64_t k = d[i];
        k *= m; k ^= k >> r; k *= m;
        h ^= k;
        h *= m;
    }
    const uint8_t* d2 = data + n*8;
    switch(len & 7) {
        case 7: h ^= uint64_t(d2[6]) << 48;
        case 6: h ^= uint64_t(d2[5]) << 40;
        case 5: h ^= uint64_t(d2[4]) << 32;
        case 4: h ^= uint64_t(d2[3]) << 24;
        case 3: h ^= uint64_t(d2[2]) << 16;
        case 2: h ^= uint64_t(d2[1]) << 8;
        case 1: h ^= uint64_t(d2[0]); h *= m;
    }
    h ^= h >> r;
    h *= m;
    h ^= h >> r;
    return static_cast<size_t>(h);
}

template <size_t N>
struct BitSet {
    static constexpr size_t NUM_BYTES = (N + 7) / 8;
    std::array<uint8_t, NUM_BYTES> bytes{};

    size_t GetSize() const { return N; }
    uint8_t GetByte(size_t idx) const { return bytes[idx]; }
    const uint8_t* GetBytes() const { return bytes.data(); }

    bool operator==(const BitSet& other) const {
        return bytes == other.bytes;
    }
};

// utility to randomize a vector of BitSets
template <size_t N>
void randomize(std::vector<BitSet<N>>& vec) {
    std::mt19937 rng(12345);
    std::uniform_int_distribution<int> dist(0,255);
    for(auto& bs : vec) {
        for(size_t i=0;i<BitSet<N>::NUM_BYTES; ++i)
            bs.bytes[i] = static_cast<uint8_t>(dist(rng));
    }
}

} // namespace bench238

#endif
