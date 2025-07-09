#include <chrono>
#include <iostream>
#include <string>

#include "minicurl.hpp"

std::string getURL(MiniCurl& curl, size_t size);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    size_t size = 20 * 1024 * 1024; // default payload size: 20MB
    int repeat = 5000;              // default iterations for perf

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--size=", 0) == 0)
            size = std::stoul(arg.substr(7));
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stoi(arg.substr(9));
    }

    MiniCurl curl;

    auto run_once = [&](MiniCurl& c) {
        std::string result = getURL(c, size);
        return result.size();
    };

    if (mode == "correct") {
        size_t out = run_once(curl);
        std::cout << out << '\n';
        return 0;
    }

    using clock = std::chrono::high_resolution_clock;
    size_t accum = 0;
    auto t1 = clock::now();
    for (int i = 0; i < repeat; ++i)
        accum += run_once(curl);
    auto t2 = clock::now();
    // Prevent optimizing away
    if (accum == 0) std::cerr << "";

    std::cout << "Time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
              << " ms\n";
    return 0;
}
