#include <chrono>
#include <iostream>
#include <string>

#include "graph.h"

long resolve(const ds::Graph&, int);

ds::Graph createGraph(std::size_t count) {
    // Each graph has the same amount of regular, call and aux nodes
    return ds::Graph(count, count, count);
}

int main(int argc, char* argv[]) {
    std::string mode = "perf"; // correct or perf
    int repeat = 30000; // workload repetitions
    std::size_t nodes = 1000; // number of nodes per category

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--repeat=", 0) == 0) repeat = std::stoi(arg.substr(9));
        else if (arg.rfind("--nodes=", 0) == 0) nodes = static_cast<std::size_t>(std::stoul(arg.substr(8)));
    }

    ds::Graph src = createGraph(nodes);

    if (mode == "correct") {
        long val = resolve(src, 1);
        std::cout << val << "\n";
        return 0;
    }

    using clock = std::chrono::high_resolution_clock;
    auto t1 = clock::now();
    resolve(src, repeat);
    auto t2 = clock::now();
    std::cout << "Time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
              << " ms\n";
}
