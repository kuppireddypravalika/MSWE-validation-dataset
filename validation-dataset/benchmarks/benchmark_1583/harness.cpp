#include "cell.hpp"
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

void computeDepthRelatedResults(const MainGrid& grid,
                                const ActiveCellInfo& info,
                                std::vector<double>& depth,
                                std::vector<double>& dx,
                                std::vector<double>& dy,
                                std::vector<double>& dz,
                                std::vector<double>& tops,
                                std::vector<double>& bottom);

// Create a grid with `n` cells arranged along the x-axis
MainGrid makeGrid(std::size_t n) {
    static Grid mainGrid{false};
    MainGrid g;
    g.cells.reserve(n);
    for (std::size_t i = 0; i < n; ++i) {
        g.cells.emplace_back(&mainGrid, Vec3(static_cast<double>(i), 0.0, static_cast<double>(i % 100)));
    }
    return g;
}

ActiveCellInfo makeActiveInfo(std::size_t n) {
    ActiveCellInfo info;
    info.mapping.resize(n);
    for (std::size_t i = 0; i < n; ++i) info.mapping[i] = i;
    return info;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    std::size_t cells = 1000000; // default number of cells
    int repeat = 1000;           // number of iterations

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--cells=", 0) == 0) cells = std::stoul(arg.substr(8));
        else if (arg.rfind("--repeat=", 0) == 0) repeat = std::stoi(arg.substr(9));
    }

    MainGrid grid = makeGrid(cells);
    ActiveCellInfo info = makeActiveInfo(cells);
    std::vector<double> depth, dx, dy, dz, tops, bottom;

    if (mode == "correct") {
        computeDepthRelatedResults(grid, info, depth, dx, dy, dz, tops, bottom);
        double sum = 0.0;
        for (double v : depth) sum += v;
        for (double v : dx) sum += v;
        for (double v : dy) sum += v;
        for (double v : dz) sum += v;
        for (double v : tops) sum += v;
        for (double v : bottom) sum += v;
        std::cout << sum << '\n';
        return 0;
    }

    using clock = std::chrono::high_resolution_clock;
    auto t1 = clock::now();
    for (int r = 0; r < repeat; ++r)
        computeDepthRelatedResults(grid, info, depth, dx, dy, dz, tops, bottom);
    auto t2 = clock::now();

    std::cout << "Time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
              << " ms\n";

    if (depth.size() == 123456789) std::cerr << "";
    return 0;
}

