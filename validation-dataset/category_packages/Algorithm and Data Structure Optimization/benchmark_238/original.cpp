#include "bitset_common.h"
#include <unordered_set>
#include <vector>

using namespace bench238;

namespace std {
    template <size_t N>
    struct hash<bench238::BitSet<N>> {
        size_t operator()(const bench238::BitSet<N>& bs) const {
            static const uint32_t NUM_BYTES = 1 + ((bs.GetSize() - 1) >> 3);
            size_t result = bs.GetByte(0);
            for (unsigned int i = 1; i < NUM_BYTES; ++i) {
                result = bench238::hash_combine(result, bs.GetByte(i));
            }
            return result;
        }
    };
}

template <size_t N>
size_t process(const std::vector<BitSet<N>>& vec) {
    std::unordered_set<BitSet<N>> set;
    for (const auto& bs : vec) {
        set.insert(bs);
    }
    return set.size();
}

// explicit instantiation for 256-bit bitsets
template size_t process<256>(const std::vector<BitSet<256>>&);
