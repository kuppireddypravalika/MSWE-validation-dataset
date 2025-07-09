#include <vector>
#include <algorithm>
#include <numeric>

static constexpr int DIVISOR = 5;
static constexpr int MODULO = 1000;

static int convertResToCOFF(const std::vector<int>& files) {
    return std::accumulate(files.begin(), files.end(), 0) % MODULO;
}

long long process(std::vector<int> inputs) {
    auto IsResource = [](int v) { return v % DIVISOR == 0; };
    auto It = std::stable_partition(inputs.begin(), inputs.end(), IsResource);
    if (It != inputs.begin()) {
        std::vector<int> Files(inputs.begin(), It);
        // Preallocate the space to avoid reallocations
        inputs.erase(inputs.begin(), It);
        inputs.push_back(convertResToCOFF(Files));
    }
    return std::accumulate(inputs.begin(), inputs.end(), 0LL);
}
