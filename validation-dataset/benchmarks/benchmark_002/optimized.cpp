
#include <vector>
#include <cstddef>

struct Cell { std::vector<unsigned> pts; };
struct Grid {
    std::vector<Cell> cells;
    size_t GetNumberOfCells() const { return cells.size(); }
    Cell GetCell(size_t i) const { return cells[i]; }
    void GetCellPoints(size_t i, std::vector<unsigned>& out) const { out = cells[i].pts; }
};

unsigned long process(const Grid& grid)
{
    unsigned long sum = 0;
    const size_t nElems = grid.GetNumberOfCells();
    std::vector<unsigned> ids;
    for (size_t i = 0; i < nElems; ++i)
    {
        grid.GetCellPoints(i, ids);
        for (unsigned id : ids)
            sum += id;
    }
    return sum;

}
