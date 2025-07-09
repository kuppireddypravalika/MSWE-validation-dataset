#include <vector>
#include <algorithm>
#include <numeric>

static int convertResToCOFF(const std::vector<int>& files) {
    int sum = 0;
    for (int v : files) sum += v;
    return sum % 1000;
}

long long process(std::vector<int> inputs) {
    auto IsResource = [](int v) { return v % 5 == 0; };
    auto It = std::stable_partition(inputs.begin(), inputs.end(), IsResource);
    if (It != inputs.begin()) {
        std::vector<int> Files(inputs.begin(), It);
        int converted = convertResToCOFF(Files);
        inputs = std::vector<int>(It, inputs.end());
        inputs.push_back(converted);
    }
    return std::accumulate(inputs.begin(), inputs.end(), 0LL);
}
