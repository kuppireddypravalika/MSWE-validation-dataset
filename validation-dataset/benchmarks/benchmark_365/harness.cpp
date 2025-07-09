#include <chrono>
#include <iostream>
#include <string>
#include <vector>

std::size_t run(int repeat, std::size_t vec_size);

int main(int argc, char* argv[])
{
    std::string mode = "perf";
    int repeat = 5000000; // workload scaled for ~10s original runtime
    std::size_t vec_size = 10000;

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--repeat=", 0) == 0) repeat = std::stoi(arg.substr(9));
        else if (arg.rfind("--size=", 0) == 0) vec_size = std::stoul(arg.substr(7));
    }

    if (mode == "correct")
    {
        std::size_t result = run(1, 4); // small inputs for correctness
        std::cout << result << '\n';
    }
    else
    {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        run(repeat, vec_size);
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }

    return 0;
}
