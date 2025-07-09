#include <vector>
#include <algorithm>
#include <numeric>

static int convertResToCOFF(const std::vector<int>& files) {
    return std::accumulate(files.begin(), files.end(), 0) % 1000;
}

long long process(std::vector<int> inputs) {
    auto IsResource = [](int v) { return v % 5 == 0; };
    auto It = std::stable_partition(inputs.begin(), inputs.end(), IsResource);
    if (It != inputs.begin()) {
        int converted = convertResToCOFF(std::vector<int>(inputs.begin(), It));
        inputs.erase(inputs.begin(), It);
        inputs.push_back(converted);
    }
    return std::accumulate(inputs.begin(), inputs.end(), 0LL);
}
