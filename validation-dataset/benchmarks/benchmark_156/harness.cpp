#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>

struct Func { std::string name; int value; };
using CalleeVec = std::vector<const Func*>;
using IndMap = std::vector<CalleeVec>;
volatile size_t sink = 0;

IndMap create_indmap(const std::vector<Func>& funcs, size_t sites, size_t entries_per_site) {
    IndMap map(sites);
    size_t unique_funcs = funcs.size() / 2;
    for(size_t s=0; s<sites; ++s) {
        auto& vec = map[s];
        vec.reserve(entries_per_site);
        for(size_t j=0; j<entries_per_site; ++j) {
            size_t idx = (j % unique_funcs) * 2 + (j % 2);
            vec.push_back(&funcs[idx]);
        }
    }
    return map;
}

std::vector<Func> create_funcs(size_t unique_funcs) {
    std::vector<Func> funcs;
    funcs.reserve(unique_funcs * 2);
    for(size_t i=0; i<unique_funcs; ++i) {
        funcs.push_back({"F" + std::to_string(i), (int)i});
        funcs.push_back({"F" + std::to_string(i), (int)i});
    }
    return funcs;
}


size_t process_indmap(IndMap indmap, int work);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    size_t sites = 1000;
    size_t entries = 200;
    size_t unique_funcs = 50;
    int work = 2000;
    int repeat = 100;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--sites=",0) == 0) sites = std::stoul(arg.substr(8));
        else if (arg.rfind("--entries=",0) == 0) entries = std::stoul(arg.substr(10));
        else if (arg.rfind("--unique=",0) == 0) unique_funcs = std::stoul(arg.substr(9));
        else if (arg.rfind("--work=",0) == 0) work = std::stoi(arg.substr(7));
        else if (arg.rfind("--repeat=",0) == 0) repeat = std::stoi(arg.substr(9));
    }

    auto funcs = create_funcs(unique_funcs);
    auto base = create_indmap(funcs, sites, entries);

    if (mode == "correct") {

        size_t result = process_indmap(base, work);
        std::cout << result << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for(int r=0;r<repeat;++r) {
            auto data = base;
            process_indmap(data, work);
        }
        auto t2 = clock::now();

        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms\n";
    }
    return 0;
}
