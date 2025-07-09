#include "drawing.hpp"
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

void filter_rect_wrapper(rct_drawpixelinfo* dpi);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    std::string impl = "original";
    int repeat = 40000;
    int size = 512;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--impl=",0)==0) impl = arg.substr(8);
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
        else if (arg.rfind("--size=",0)==0) size = std::stoi(arg.substr(7));
    }

    init_g1();

    rct_drawpixelinfo dpi{size, size, 0, 0, 0, 0, nullptr};
    std::vector<uint8> base(static_cast<size_t>(size) * size);
    for (size_t i = 0; i < base.size(); ++i) base[i] = static_cast<uint8>(i % 256);
    std::vector<uint8> buffer = base;
    dpi.bits = buffer.data();

    if (mode == "correct") {
        if (impl == "optimized")
            filter_rect_wrapper(&dpi); // call optimized after linking optimized version
        else
            filter_rect_wrapper(&dpi);
        uint64_t sum = 0;
        for (uint8 v : buffer) sum += v;
        std::cout << sum << "\n";
    } else {
        using clock = std::chrono::high_resolution_clock;
        uint64_t dummy = 0;
        auto t1 = clock::now();
        for (int i = 0; i < repeat; ++i) {
            std::memcpy(buffer.data(), base.data(), base.size());
            dpi.bits = buffer.data();
            filter_rect_wrapper(&dpi);
            dummy += buffer[0];
        }
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
        if (dummy == 123456789) std::cerr << "";
    }

    return 0;
}
