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

        if (depth.size() == actCellCount || isTemporaryGrid) depth[resultIndex] = std::abs(cell.center().z);

        if (dx.size() == actCellCount || isTemporaryGrid) {
            dx[resultIndex] = std::abs(cell.faceCenter(POS_I).x - cell.faceCenter(NEG_I).x);
        }

        if (dy.size() == actCellCount || isTemporaryGrid) {
            dy[resultIndex] = std::abs(cell.faceCenter(POS_J).y - cell.faceCenter(NEG_J).y);
        }

        if (dz.size() == actCellCount || isTemporaryGrid) {
            dz[resultIndex] = std::abs(cell.faceCenter(POS_K).z - cell.faceCenter(NEG_K).z);
        }

        if (tops.size() == actCellCount || isTemporaryGrid) tops[resultIndex] = std::abs(cell.faceCenter(NEG_K).z);

        if (bottom.size() == actCellCount || isTemporaryGrid) bottom[resultIndex] = std::abs(cell.faceCenter(POS_K).z);
    }
}

// Explicit template instantiations