#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <vector>

struct BasicSet { int val; };
using Set = std::vector<BasicSet>;
using UnionSet = std::vector<Set>;

long long process(const UnionSet &USet);

UnionSet createData(int nSets, int setSize) {
    UnionSet uset(nSets);
    std::mt19937 rng(123);
    std::uniform_int_distribution<int> dist(0, 1000);
    for (int i = 0; i < nSets; ++i) {
        uset[i].resize(setSize);
        for (int j = 0; j < setSize; ++j)
            uset[i][j].val = dist(rng);
    }
    return uset;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int repeat = 130000;
    int nSets = 10;
    int setSize = 100;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stoi(arg.substr(9));
        else if (arg.rfind("--sets=", 0) == 0)
            nSets = std::stoi(arg.substr(7));
        else if (arg.rfind("--size=", 0) == 0)
            setSize = std::stoi(arg.substr(7));
    }

    UnionSet data = createData(nSets, setSize);

    if (mode == "correct") {
        long long result = process(data);
        std::cout << result << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;

        auto t1 = clock::now();
        for (int r = 0; r < repeat; ++r)
            process(data);
        auto t2 = clock::now();

        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
}
