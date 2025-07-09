#include "payload.h"
#include <chrono>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int repeat = 90000;
    int entries = 600;
    int strlen = 512;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--repeat=",0) == 0) repeat = std::stoi(arg.substr(9));
        else if (arg.rfind("--entries=",0) == 0) entries = std::stoi(arg.substr(10));
        else if (arg.rfind("--strlen=",0) == 0) strlen = std::stoi(arg.substr(9));
    }

    SetupPayload a = makePayload(entries, strlen);
    SetupPayload b = makePayload(entries, strlen);
    bool result = false;

    if (mode == "correct") {
        result = equals(a, b);
        std::cout << result << std::endl;
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int i = 0; i < repeat; ++i) {
            equals(a, b);
        }
        auto t2 = clock::now();

        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms\n";
    }

    return 0;
}
