#include <chrono>
#include <iostream>
#include <string>
#include <vector>


struct Cell { std::vector<unsigned> pts; };
struct Grid {
    std::vector<Cell> cells;
    size_t GetNumberOfCells() const { return cells.size(); }
    Cell GetCell(size_t i) const { return cells[i]; }
    void GetCellPoints(size_t i, std::vector<unsigned>& out) const { out = cells[i].pts; }
};

unsigned long process(const Grid& grid);

Grid createGrid(size_t n)
{
    Grid g;
    g.cells.resize(n);
    for (size_t i = 0; i < n; ++i)
    {
        g.cells[i].pts.resize(8);
        for (size_t j = 0; j < 8; ++j)
            g.cells[i].pts[j] = static_cast<unsigned>(i * 8 + j);
    }
    return g;
}

int main(int argc, char* argv[])
{
    std::string mode = "perf";
    size_t elems = 10000;
    int repeat = 5200;

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--elems=", 0) == 0)
            elems = std::stoul(arg.substr(8));
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stoi(arg.substr(9));
    }

    Grid g = createGrid(elems);

    if (mode == "correct")
    {
        unsigned long sum = process(g);
        std::cout << sum << '\n';
    }
    else
    {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int r = 0; r < repeat; ++r)
            process(g);
        auto t2 = clock::now();

        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms\n";
    }
}
