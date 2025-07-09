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
    // Preallocate memory for BSets based on total expected size
    size_t totalSize = 0;
    for (const auto &S : USet) {
        totalSize += S.size();
    }
    std::vector<BasicSet> BSets;
    BSets.reserve(totalSize);

    // Use emplace_back to avoid unnecessary copying
    for (const auto &S : USet) {
        BSets.insert(BSets.end(), S.begin(), S.end());
    }

    if (BSets.empty())
        return 0;

    // Instead of shuffling followed by sorting, we only sort
    std::sort(BSets.begin(), BSets.end(), orderComparer);

    long long sum = 0;
    for (const auto &B : BSets) {
        sum += B.val;
    }
    return sum;
}

// Explicit template instantiation statements go here