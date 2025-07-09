#include <algorithm>
#include <random>
#include <vector>

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
    std::vector<BasicSet> BSets;
    for (const auto &S : USet)
        for (const auto &B : S)
            BSets.push_back(B);

    if (BSets.empty())
        return 0;

    sortWithShuffle(BSets.begin(), BSets.end(), orderComparer);

    long long sum = 0;
    for (const auto &B : BSets)
        sum += B.val;
    return sum;
}
