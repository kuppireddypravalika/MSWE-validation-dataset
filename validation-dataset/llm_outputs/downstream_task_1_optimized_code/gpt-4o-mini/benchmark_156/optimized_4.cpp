#include <vector>
#include <string>
#include <algorithm>
#include <set>

struct Func { std::string name; int value; };
using CalleeVec = std::vector<const Func*>;
using IndMap = std::vector<CalleeVec>;

extern volatile size_t sink;

size_t process_indmap(IndMap indmap, int work) {
    size_t result = 0;
    for(auto &callees : indmap) {
        // Use a set to maintain uniqueness while sorting
        std::set<const Func*, decltype(&Func::name)> uniqueCallees(callees.begin(), callees.end(), [](const Func* a, const Func* b){ return a->name < b->name; });

        // Perform the computation on all elements
        for(const Func* f : uniqueCallees) {
            for(int i = 0; i < work; ++i)
                sink += f->value + i;
            result += f->value; // Accumulate value in one go
        }
    }
    return result;
}