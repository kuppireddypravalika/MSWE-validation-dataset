#include "recycling.hpp"
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

void updateBiowasteRecycling(std::vector<Residence*>& residences,
                             std::vector<Recycling*>& recyclingFacilities);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    std::string impl = "original";
    int numRes = 100;
    int numRec = 100000;
    int repeat = 120000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg == "--impl=optimized") impl = "optimized";
        else if (arg == "--impl=original") impl = "original";
        else if (arg.rfind("--residences=",0)==0) numRes = std::stoi(arg.substr(13));
        else if (arg.rfind("--recycling=",0)==0) numRec = std::stoi(arg.substr(12));
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
    }

    std::vector<Residence> res_store(numRes);
    std::vector<Residence*> residences;
    residences.reserve(numRes);
    for (auto& r : res_store) residences.push_back(&r);

    std::vector<Recycling> rec_store(numRec);
    std::vector<Recycling*> recyclers;
    recyclers.reserve(numRec);
    for (auto& r : rec_store) recyclers.push_back(&r);

    if (mode == "correct") {
        if (impl == "optimized")
            updateBiowasteRecycling(residences, recyclers);
        else
            updateBiowasteRecycling(residences, recyclers);
        long long sum = 0;
        for (auto* r : residences) sum += r->waste;
        std::cout << sum << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int i = 0; i < repeat; ++i) {
            for (auto& r : res_store) r.waste = 100;
            updateBiowasteRecycling(residences, recyclers);
        }
        auto t2 = clock::now();
        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms\n";
    }
    return 0;
}
