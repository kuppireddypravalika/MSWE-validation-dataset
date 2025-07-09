#include <vector>
#include <cstddef>

struct Cell { std::vector<unsigned> pts; };
struct Grid {
    std::vector<Cell> cells;
    size_t GetNumberOfCells() const { return cells.size(); }
    const Cell& GetCell(size_t i) const { return cells[i]; } // returns by reference
};

unsigned long process(const Grid& grid)
{
    unsigned long sum = 0;
    const size_t nElems = grid.GetNumberOfCells();
    for (size_t i = 0; i < nElems; ++i)
    {
        const Cell& cell = grid.GetCell(i);
        const std::vector<unsigned>& pts = cell.pts;
        for (unsigned point : pts)
            sum += point;
    }
    return sum;
}

// Explicit template instantiation statements would go here.
