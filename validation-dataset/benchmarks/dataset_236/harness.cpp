#include "styled_renderable.hpp"
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

static void callback(int& c) { ++c; }

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    std::size_t count = 200;
    int repeat = 1000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--count=",0)==0) count = static_cast<std::size_t>(std::stoul(arg.substr(8)));
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
    }

    EventManager mgr;
    std::vector<StyledRenderable> items(count);
    for (std::size_t i = 0; i < count; ++i) {
        items[i].Manager = &mgr;
        items[i].OnActivate = (i % 10 == 0) ? callback : nullptr;
        items[i].OnDeactivate = (i % 15 == 0) ? callback : nullptr;
        items[i].Handle = static_cast<int>(i);
    }

    if (mode == "correct") {
        for (auto& it : items) render_item(it);
        std::cout << mgr.counter << "\n";
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int r = 0; r < repeat; ++r) {
            for (auto& it : items) render_item(it);
        }
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
    return 0;
}
