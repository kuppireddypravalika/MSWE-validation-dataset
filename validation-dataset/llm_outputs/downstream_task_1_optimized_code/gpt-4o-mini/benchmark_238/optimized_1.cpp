#include "bitset_common.h"
#include <unordered_set>
#include <vector>

using namespace bench238;

namespace std {
    template <size_t N>
    struct hash<bench238::BitSet<N>> {
        size_t operator()(const bench238::BitSet<N>& bs) const {
            size_t result = 0;
            for (unsigned int i = 0; i < BitSet<N>::NUM_BYTES; ++i) {
                result ^= (result << 5) + (result >> 2) + bs.GetByte(i);
            }
            return result;
        }
    };
}

template <size_t N>
size_t process(const std::vector<BitSet<N>>& vec) {
    std::unordered_set<BitSet<N>> set;
    set.reserve(vec.size()); // Pre-allocate memory for performance
    set.insert(vec.begin(), vec.end()); // Insert all elements in one go
    return set.size();
}

// explicit instantiation for 256-bit bitsets
template size_t process<256>(const std::vector<BitSet<256>>&);