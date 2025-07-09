#include <chrono>
#include <iostream>
#include <string>
#include <vector>

void initgdt(volatile int*, const int*, const int*, std::size_t, int);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int repeat = 9000000;
    std::size_t size = 1024;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--repeat=", 0) == 0) repeat = std::stoi(arg.substr(9));
        else if (arg.rfind("--size=", 0) == 0) size = std::stoul(arg.substr(7));
    }

    std::vector<int> src1(size, 1), src2(size, 2);
    std::vector<int> dest(size, 0);

    if (mode == "correct") {
        initgdt(dest.data(), src1.data(), src2.data(), size, 1);
        for (std::size_t i = 0; i < dest.size(); ++i)
            std::cout << dest[i] << ' ';
        std::cout << std::endl;
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        initgdt(dest.data(), src1.data(), src2.data(), size, repeat);
        auto t2 = clock::now();

        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }

    return 0;
}
