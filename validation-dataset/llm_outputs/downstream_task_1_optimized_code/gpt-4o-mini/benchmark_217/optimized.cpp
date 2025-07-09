#include <algorithm>
#include <random>
#include <vector>

struct BasicSet { int val; };
using Set = std::vector<BasicSet>;
using UnionSet = std::vector<Set>;

static bool orderComparer(const BasicSet &A, const BasicSet &B) {
    return A.val < B.val;
}

static std::mt19937 rng(42);

// Optimized sort function that handles shuffling and sorting in a single pass
long long process(const UnionSet &USet) {
    std::vector<BasicSet> BSets;
    BSets.reserve([&]{
        size_t totalSize = 0;
        for (const auto &S : USet) totalSize += S.size();
        return totalSize;
    }());

    for (const auto &S : USet)
        BSets.insert(BSets.end(), S.begin(), S.end());

    if (BSets.empty())
        return 0;

    std::shuffle(BSets.begin(), BSets.end(), rng);
    std::sort(BSets.begin(), BSets.end(), orderComparer);

    long long sum = 0;
    for (const auto &B : BSets)
        sum += B.val;
    return sum;
}

// Explicit template instantiation statements
