#include "cell.hpp"
#include <vector>
#include <cmath>
#include <limits>

void computeDepthRelatedResults(const MainGrid& grid,
                                const ActiveCellInfo& info,
                                std::vector<double>& depth,
                                std::vector<double>& dx,
                                std::vector<double>& dy,
                                std::vector<double>& dz,
                                std::vector<double>& tops,
                                std::vector<double>& bottom)
{
    std::size_t actCellCount = info.reservoirActiveCellCount();
    if (actCellCount == 0) return;

    // Ensure resizing of result vectors based on active cell count
    if (depth.size() < actCellCount) depth.resize(actCellCount, std::numeric_limits<double>::max());
    if (dx.size() < actCellCount) dx.resize(actCellCount, std::numeric_limits<double>::max());
    if (dy.size() < actCellCount) dy.resize(actCellCount, std::numeric_limits<double>::max());
    if (dz.size() < actCellCount) dz.resize(actCellCount, std::numeric_limits<double>::max());
    if (tops.size() < actCellCount) tops.resize(actCellCount, std::numeric_limits<double>::max());
    if (bottom.size() < actCellCount) bottom.resize(actCellCount, std::numeric_limits<double>::max());

    for (std::size_t cellIdx = 0; cellIdx < grid.cells.size(); ++cellIdx) {
        const RigCell& cell = grid.cells[cellIdx];
        std::size_t resultIndex = info.cellResultIndex(cellIdx);
        if (resultIndex == UNDEFINED_SIZE_T) continue;

        bool isTemporaryGrid = cell.hostGrid()->isTempGrid();

        depth[resultIndex] = (depth.size() == actCellCount || isTemporaryGrid) ? std::abs(cell.center().z) : depth[resultIndex];
        dx[resultIndex] = (dx.size() == actCellCount || isTemporaryGrid) ? std::abs(cell.faceCenter(POS_I).x - cell.faceCenter(NEG_I).x) : dx[resultIndex];
        dy[resultIndex] = (dy.size() == actCellCount || isTemporaryGrid) ? std::abs(cell.faceCenter(POS_J).y - cell.faceCenter(NEG_J).y) : dy[resultIndex];
        dz[resultIndex] = (dz.size() == actCellCount || isTemporaryGrid) ? std::abs(cell.faceCenter(POS_K).z - cell.faceCenter(NEG_K).z) : dz[resultIndex];
        tops[resultIndex] = (tops.size() == actCellCount || isTemporaryGrid) ? std::abs(cell.faceCenter(NEG_K).z) : tops[resultIndex];
        bottom[resultIndex] = (bottom.size() == actCellCount || isTemporaryGrid) ? std::abs(cell.faceCenter(POS_K).z) : bottom[resultIndex];
    }
}

// Explicit template instantiations