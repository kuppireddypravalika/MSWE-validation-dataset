#include <algorithm>
#include <random>
#include <vector>

struct BasicSet { int val; };
using Set = std::vector<BasicSet>;
using UnionSet = std::vector<Set>;

static bool orderComparer(const BasicSet &A, const BasicSet &B) {
    return A.val < B.val;
}

long long process(const UnionSet &USet) {
    // Reserve space in BSets to avoid multiple allocations
    std::vector<BasicSet> BSets;
    size_t totalSize = 0;
    for (const auto &S : USet) {
        totalSize += S.size();
    }
    BSets.reserve(totalSize);

    for (const auto &S : USet) {
        BSets.insert(BSets.end(), S.begin(), S.end());
    }

    if (BSets.empty())
        return 0;

    // Use std::shuffle directly with a more modern approach
    static std::mt19937 rng(42);
    std::shuffle(BSets.begin(), BSets.end(), rng);
    std::sort(BSets.begin(), BSets.end(), orderComparer);

    long long sum = 0;
    // Use range-based for loop to iterate
    for (const auto &B : BSets) {
        sum += B.val;
    }
    return sum;
}

// Explicit template instantiation statements

