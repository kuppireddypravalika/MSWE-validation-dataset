#include <chrono>
#include <iostream>
#include <string>
#include <vector>

uint64_t process(const std::vector<uint32_t>& data);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    size_t elems = 1'000'000;
    int repeat = 4000; // chosen so original runs >10s

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--repeat=", 0) == 0) repeat = std::stoi(arg.substr(9));
        else if (arg.rfind("--elems=", 0) == 0) elems = std::stoul(arg.substr(8));
    }

    std::vector<uint32_t> data(elems);
    for (size_t i = 0; i < elems; ++i)
        data[i] = static_cast<uint32_t>(i) * 1234567u + 0x89abcdefu;

    if (mode == "correct") {
        uint64_t sum = process(data);
        std::cout << sum << '\n';
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
    return 0;
}
