#include <vector>
#include <string>
#include <algorithm>

struct Func { std::string name; int value; };
using CalleeVec = std::vector<const Func*>;
using IndMap = std::vector<CalleeVec>;

extern volatile size_t sink;

size_t process_indmap(IndMap indmap, int work) {
    auto NameCmp = [](const Func* a, const Func* b) { return a->name < b->name; };
    size_t result = 0;
    for (auto& callees : indmap) {
        // Sort and remove duplicates in a single pass using std::stable_sort
        std::stable_sort(callees.begin(), callees.end(), NameCmp);
        auto last = std::unique(callees.begin(), callees.end(), [](const Func* a, const Func* b) { return a->name == b->name; });
        callees.erase(last, callees.end());

        // Compute contributions
        for (const Func* f : callees) {
            size_t value = f->value;
            sink += value * work * (work - 1) / 2 + work * value; // Optimized contributions calculation
        }

        // Compute final result using unique elements directly from the callees vector
        for (const Func* f : callees) {
            result += f->value;
        }
    }
    return result;
}
