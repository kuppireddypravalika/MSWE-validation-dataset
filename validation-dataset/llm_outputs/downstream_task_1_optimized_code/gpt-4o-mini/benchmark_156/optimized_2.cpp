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
        // Use a set to avoid sorting and handle uniqueness more efficiently
        std::unordered_set<std::string> uniqueNames;
        for(const Func* f : callees) {
            uniqueNames.insert(f->name);
            for(int i = 0; i < work; ++i)
                sink += f->value + i;
        }
        // Process unique elements
        for(const std::string &name : uniqueNames) {
            // Find the first Func that matches the name
            auto it = std::find_if(callees.begin(), callees.end(), [&name](const Func* f) { return f->name == name; });
            if (it != callees.end()) {
                result += (*it)->value;
            }
        }
    }
    return result;
}

// Explicit template instantiation statements
