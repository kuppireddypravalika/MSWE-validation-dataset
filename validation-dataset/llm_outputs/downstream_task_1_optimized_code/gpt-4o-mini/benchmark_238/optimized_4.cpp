#include "bitset_common.h"
#include <unordered_set>
#include <vector>

using namespace bench238;

namespace std {
    template <size_t N>
    struct hash<bench238::BitSet<N>> {
        size_t operator()(const bench238::BitSet<N>& bs) const {
            size_t result = 0;
            const size_t* data = reinterpret_cast<const size_t*>(bs.GetBytes());
            for (size_t i = 0; i < BitSet<N>::NUM_BYTES / sizeof(size_t); ++i) {
                result = bench238::hash_combine(result, data[i]);
            }
            // Handle any remaining bytes that couldn't be represented as size_t
            for (size_t i = (BitSet<N>::NUM_BYTES / sizeof(size_t)) * sizeof(size_t); i < BitSet<N>::NUM_BYTES; ++i) {
                result = bench238::hash_combine(result, bs.GetByte(i));
            }
            return result;
        }
    };
}

template <size_t N>
size_t process(const std::vector<BitSet<N>>& vec) {
    std::unordered_set<BitSet<N>> set(vec.begin(), vec.end());
    return set.size();
}

// explicit instantiation for 256-bit bitsets
template size_t process<256>(const std::vector<BitSet<256>>&);