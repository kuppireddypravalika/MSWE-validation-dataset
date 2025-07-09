#include "bitset_common.h"
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

using namespace bench238;

template <size_t N>
size_t process(const std::vector<BitSet<N>>& vec);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    size_t count = 2500; // number of bitsets
    int repeat = 24000;  // iterations

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--count=",0) == 0) count = std::stoul(arg.substr(8));
        else if (arg.rfind("--repeat=",0) == 0) repeat = std::stoi(arg.substr(9));
    }

    std::vector<BitSet<256>> data(count);
    randomize(data);

    if (mode == "correct") {
        size_t result = process<256>(data);
        std::cout << result << '\n';
        return 0;
    }

    using clock = std::chrono::high_resolution_clock;
    auto t1 = clock::now();
    for (int i = 0; i < repeat; ++i) process<256>(data);
    auto t2 = clock::now();

    std::cout << "Time: " <<
        std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
              << " ms\n";
    return 0;
}
