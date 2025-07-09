#include <vector>
#include <string>
#include <algorithm>

struct Func { std::string name; int value; };
using CalleeVec = std::vector<const Func*>;
using IndMap = std::vector<CalleeVec>;

extern volatile size_t sink;


size_t process_indmap(IndMap indmap, int work) {
    auto NameCmp = [](const Func* a, const Func* b){ return a->name < b->name; };
    size_t result = 0;
    for(auto &callees : indmap) {
        std::sort(callees.begin(), callees.end(), NameCmp);
        callees.erase(std::unique(callees.begin(), callees.end(), NameCmp), callees.end());
        for(const Func* f : callees)
            for(int i=0;i<work;++i)
                sink += f->value + i;
        for(const Func* f : callees)
            result += f->value;
    }
    return result;
}
