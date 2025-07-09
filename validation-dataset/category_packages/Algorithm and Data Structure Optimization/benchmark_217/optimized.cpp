#include <algorithm>
#include <vector>

struct BasicSet { int val; };
using Set = std::vector<BasicSet>;
using UnionSet = std::vector<Set>;

static bool orderComparer(const BasicSet &A, const BasicSet &B) {
    return A.val < B.val;
}

namespace llvm {
    template <class RandomIt, class Compare>
    inline void sort(RandomIt first, RandomIt last, Compare comp) {
        std::sort(first, last, comp);
    }
}

long long process(const UnionSet &USet) {
    std::vector<BasicSet> BSets;
    for (const auto &S : USet)
        for (const auto &B : S)
            BSets.push_back(B);

    if (BSets.empty())
        return 0;

    llvm::sort(BSets.begin(), BSets.end(), orderComparer);

    long long sum = 0;
    for (const auto &B : BSets)
        sum += B.val;
    return sum;
}
