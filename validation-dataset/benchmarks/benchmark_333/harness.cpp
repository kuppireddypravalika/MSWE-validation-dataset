#include <chrono>
#include <iostream>
#include <string>

int emit_epilogue(bool, int, int); // provided by original.cpp or optimized.cpp

int main(int argc, char** argv) {
    std::string mode = "perf";
    bool isARM = false;  // Thumb2 path triggers the optimization
    int numBytes = 0;    // Zero offset case
    int repeat = 5000000; // runtime control

    for (int i=1;i<argc;++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
    }

    if (mode == "correct") {
        int a = emit_epilogue(isARM, numBytes, 10);
        std::cout << a << std::endl;
        return 0;
    }

    using clock = std::chrono::high_resolution_clock;
    auto t1 = clock::now();
    volatile int sink = 0;
    for (int i=0;i<repeat;++i) {
        sink += emit_epilogue(isARM, numBytes, 1);
    }
    auto t2 = clock::now();
    std::cout << "Time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()
              << " ms\n";
    return 0;
}
