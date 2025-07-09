#include <vector>
#include <cstddef>

struct Cell { std::vector<unsigned> pts; }; 
struct Grid {
    std::vector<Cell> cells;
    size_t GetNumberOfCells() const { return cells.size(); }
    const Cell& GetCell(size_t i) const { return cells[i]; } // change to return by const reference
};

unsigned long process(const Grid& grid)
{
    unsigned long sum = 0;
    const size_t nElems = grid.GetNumberOfCells();
    for (size_t i = 0; i < nElems; ++i)
    {
        const Cell& cell = grid.GetCell(i); // store cell in a reference
        const size_t n = cell.pts.size();
        const std::vector<unsigned>& points = cell.pts; // reference for pts
        for (size_t j = 0; j < n; ++j)
            sum += points[j]; // use points reference
    }
    return sum;
}

// Explicit template instantiation statements (if any) go here.
