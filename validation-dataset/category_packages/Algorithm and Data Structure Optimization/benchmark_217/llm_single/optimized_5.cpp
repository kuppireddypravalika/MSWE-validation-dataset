#include <algorithm>
#include <random>
#include <vector>
#include <numeric>

struct BasicSet { int val; };
using Set = std::vector<BasicSet>;
using UnionSet = std::vector<Set>;

static bool orderComparer(const BasicSet &A, const BasicSet &B) {
    return A.val < B.val;
}

// Optimized version: Directly calculating the total and resizing the BSets vector to avoid repeated allocations.
long long process(const UnionSet &USet) {
    size_t totalSize = 0;
   
    // First, calculate total size of BasicSet entries in all sets.
    for (const auto &S : USet) {
        totalSize += S.size();
    }

    if (totalSize == 0) return 0;
    
    std::vector<BasicSet> BSets;
    BSets.reserve(totalSize); // Reserve memory for the total size.

    // Copy all BasicSets into the BSets vector.
    for (const auto &S : USet) {
        BSets.insert(BSets.end(), S.begin(), S.end());
    }

    // Perform in-place shuffle followed by sort.
    std::mt19937 rng(42);
    std::shuffle(BSets.begin(), BSets.end(), rng);
    std::sort(BSets.begin(), BSets.end(), orderComparer);

    // Calculate the sum using accumulate for better performance.
    return std::accumulate(BSets.begin(), BSets.end(), 0LL, [](long long sum, const BasicSet &B) {
        return sum + B.val;
    });
}

// Explicit template instantiation statements
// (If any need to be added, include them here as seen in original files.)
