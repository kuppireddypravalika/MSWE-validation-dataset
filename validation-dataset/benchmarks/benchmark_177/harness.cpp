#include <chrono>
#include <iostream>
#include <string>

#include "dummy_pango.h"

int GetCharHeight(DummyContext* ctx, DummyFontDesc* desc);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int repeat = 10;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--repeat=",0) == 0) repeat = std::stoi(arg.substr(9));
    }

    DummyContext ctx{1};
    DummyFontDesc desc;

    if (mode == "correct") {
        int h = GetCharHeight(&ctx, &desc);
        std::cout << h << std::endl;
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int i = 0; i < repeat; ++i)
            GetCharHeight(&ctx, &desc);
        auto t2 = clock::now();

        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
    return 0;
}
