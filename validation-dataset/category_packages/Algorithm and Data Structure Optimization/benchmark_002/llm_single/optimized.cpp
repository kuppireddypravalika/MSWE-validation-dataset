#include <vector>
#include <cstddef>

struct Cell { std::vector<unsigned> pts; };
struct Grid {
    std::vector<Cell> cells;
    size_t GetNumberOfCells() const { return cells.size(); }
    const Cell& GetCell(size_t i) const { return cells[i]; } // returns by reference
};

unsigned long process(const Grid& grid) {
    unsigned long sum = 0;
    const size_t nElems = grid.GetNumberOfCells();
    for (size_t i = 0; i < nElems; ++i) {
        const std::vector<unsigned>& points = grid.GetCell(i).pts;
        for (size_t j = 0; j < points.size(); ++j) {
            sum += points[j];
        }
    }
    return sum;
}

// Explicit template instantiation statements
