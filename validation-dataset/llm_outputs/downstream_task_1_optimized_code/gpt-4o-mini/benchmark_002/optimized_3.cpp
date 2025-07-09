#include <vector>
#include <cstddef>

struct Cell { std::vector<unsigned> pts; };
struct Grid {
    std::vector<Cell> cells;
    size_t GetNumberOfCells() const { return cells.size(); }
    const Cell& GetCell(size_t i) const { return cells[i]; }
    void GetCellPoints(size_t i, std::vector<unsigned>& out) const { out = cells[i].pts; }
};

unsigned long process(const Grid& grid)
{
    unsigned long sum = 0;
    const size_t nElems = grid.GetNumberOfCells();
    for (size_t i = 0; i < nElems; ++i)
    {
        const Cell& cell = grid.GetCell(i); // Get reference to avoid copying
        const size_t n = cell.pts.size();
        for (size_t j = 0; j < n; ++j)
            sum += cell.pts[j];
    }
    return sum;
}

// Explicit template instantiation statements would be reproduced here if provided.