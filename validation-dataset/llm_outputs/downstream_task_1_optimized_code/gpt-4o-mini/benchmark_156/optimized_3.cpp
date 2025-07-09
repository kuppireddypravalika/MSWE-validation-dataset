#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>

struct Func { std::string name; int value; };
using CalleeVec = std::vector<const Func*>;
using IndMap = std::vector<CalleeVec>;

extern volatile size_t sink;

size_t process_indmap(IndMap indmap, int work) {
    auto NameCmp = [](const Func* a, const Func* b){ return a->name < b->name; };
    size_t result = 0;
    for(auto &callees : indmap) {
        // Sort and unique in one pass using unordered set
        std::unordered_set<std::string> unique_names;
        CalleeVec unique_callees;

        for(const Func* f : callees) {
            if (unique_names.insert(f->name).second) {
                unique_callees.push_back(f);
            }
        }

        // Now we can do our computations on the unique_callees
        for(const Func* f : unique_callees) {
            for(int i = 0; i < work; ++i) {
                sink += f->value + i;
            }
            result += f->value;
        }
    }
    return result;
}

// Explicit template instantiation statement
