#include <vector>
#include <string>
#include <algorithm>

struct Func { std::string name; int value; }; 
using CalleeVec = std::vector<const Func*>; 
using IndMap = std::vector<CalleeVec>;

volatile size_t sink = 0;

size_t process_indmap(IndMap indmap, int work) {
    auto NameCmp = [](const Func* a, const Func* b) { return a->name < b->name; };
    size_t result = 0;
    for (auto &callees : indmap) {
        // Sorting only once
        std::sort(callees.begin(), callees.end(), NameCmp);
        // Unique in one go, ensuring duplicates remain in original
        auto last = std::unique(callees.begin(), callees.end(), [](const Func* a, const Func* b) { return a->name == b->name; });
        for (const Func* f : callees) {
            for (int i = 0; i < work; ++i) {
                sink += f->value + i;
            }
        }
        // Compute final result using unique elements only
        result += std::accumulate(callees.begin(), last, 0, [](size_t acc, const Func* f) { return acc + f->value; });
    }
    return result;
}

size_t process(IndMap indmap, int work) {
    return original_process(indmap, work);
}

// Explicit template instantiation statements
