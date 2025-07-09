#include <algorithm>
#include <random>
#include <vector>

struct BasicSet { int val; };  
using Set = std::vector<BasicSet>;  
using UnionSet = std::vector<Set>;  

static bool orderComparer(const BasicSet &A, const BasicSet &B) {
    return A.val < B.val;
}

// Improved version of sortWithShuffle for better performance
template <class RandomIt, class Compare>
static void sortWithShuffle(RandomIt first, RandomIt last, Compare comp) {
    static std::mt19937 rng(42);
    std::vector<typename std::iterator_traits<RandomIt>::value_type> buffer(first, last);
    std::shuffle(buffer.begin(), buffer.end(), rng);
    std::sort(buffer.begin(), buffer.end(), comp);
    std::move(buffer.begin(), buffer.end(), first);
}

long long process(const UnionSet &USet) {
    if (USet.empty())
        return 0;

    // Reserve space for the maximum number of elements to avoid multiple allocations
    std::vector<BasicSet> BSets;
    size_t totalSize = 0;
    for (const auto &S : USet)
        totalSize += S.size();
    BSets.reserve(totalSize);

    for (const auto &S : USet)  
        BSets.insert(BSets.end(), S.begin(), S.end());

    // Avoid calling sort for empty vector
    if (BSets.size() > 1) 
        sortWithShuffle(BSets.begin(), BSets.end(), orderComparer);

    long long sum = 0;
    for (const auto &B : BSets)
        sum += B.val;
    return sum;
}

// Explicit template instantiation statements
