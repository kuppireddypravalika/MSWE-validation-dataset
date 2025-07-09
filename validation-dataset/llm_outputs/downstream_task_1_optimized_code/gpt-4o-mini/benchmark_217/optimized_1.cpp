#include <algorithm>
#include <random>
#include <vector>
#include <numeric> // For std::accumulate

struct BasicSet { int val; };
using Set = std::vector<BasicSet>;
using UnionSet = std::vector<Set>;

static bool orderComparer(const BasicSet &A, const BasicSet &B) {
    return A.val < B.val;
}

// Wrapper mimicking the LLVM std::sort wrapper that shuffles before sorting
template <class RandomIt, class Compare>
static void sortWithShuffle(RandomIt first, RandomIt last, Compare comp) {
    static std::mt19937 rng(42);
    std::shuffle(first, last, rng);
    std::sort(first, last, comp);
}

long long process(const UnionSet &USet) {
    // Reserve memory for all possible BasicSet elements upfront
    size_t totalSize = 0;
    for (const auto &S : USet) {
        totalSize += S.size();
    }
    std::vector<BasicSet> BSets;
    BSets.reserve(totalSize); // Avoid repeated allocations

    for (const auto &S : USet)
        BSets.insert(BSets.end(), S.begin(), S.end()); // Efficient bulk insert

    if (BSets.empty())
        return 0;

    sortWithShuffle(BSets.begin(), BSets.end(), orderComparer);

    // Use accumulate for efficient summation
    return std::accumulate(BSets.begin(), BSets.end(), 0LL, [](long long sum, const BasicSet &B) {
        return sum + B.val;
    });
}

// Explicit template instantiation statements
