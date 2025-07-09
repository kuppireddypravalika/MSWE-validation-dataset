#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>

struct Func { std::string name; int value; };
using CalleeVec = std::vector<const Func*>;
using IndMap = std::vector<CalleeVec>;

extern volatile size_t sink;

size_t process_indmap(IndMap indmap, int work) {
    size_t result = 0;
    for(auto &callees : indmap) {
        // Remove duplicates while sorting
        std::sort(callees.begin(), callees.end(), [](const Func* a, const Func* b){ return a->name < b->name; });
        callees.erase(std::unique(callees.begin(), callees.end(), [](const Func* a, const Func* b){ return a->name == b->name; }), callees.end());
        // Calculate contributions to sink and result
        for(const Func* f : callees) {
            for(int i = 0; i < work; ++i) {
                sink += f->value + i;
            }
            result += f->value;
        }
    }
    return result;
}

// Explicit template instantiation statements (if any) should be inserted here.
