#include "common.h"
#include <chrono>
#include <iostream>
#include <string>

int select_candidate(const Dataset& ds);

int main(int argc, char* argv[])
{
    std::string mode = "perf";
    long long repeat = 400000000; // heavy workload per validation requirements

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stoll(arg.substr(9));
    }

    Dataset ds = createDataset();

    if (mode == "correct")
    {
        int val = select_candidate(ds);
        std::cout << val << '\n';
        return 0;
    }

    using clock = std::chrono::high_resolution_clock;

    auto t1 = clock::now();
    for (long long i = 0; i < repeat; ++i)
        select_candidate(ds);
    auto t2 = clock::now();

    std::cout << "Time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
              << " ms\n";
    return 0;
}
