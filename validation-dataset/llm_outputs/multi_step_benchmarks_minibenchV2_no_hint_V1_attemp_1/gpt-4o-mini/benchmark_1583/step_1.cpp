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

    bool computeDepth = depth.size() < actCellCount;
    bool computeDx = dx.size() < actCellCount;
    bool computeDy = dy.size() < actCellCount;
    bool computeDz = dz.size() < actCellCount;
    bool computeTops = tops.size() < actCellCount;
    bool computeBottom = bottom.size() < actCellCount;

    if (computeDepth) depth.resize(actCellCount, std::numeric_limits<double>::max());
    if (computeDx) dx.resize(actCellCount, std::numeric_limits<double>::max());
    if (computeDy) dy.resize(actCellCount, std::numeric_limits<double>::max());
    if (computeDz) dz.resize(actCellCount, std::numeric_limits<double>::max());
    if (computeTops) tops.resize(actCellCount, std::numeric_limits<double>::max());
    if (computeBottom) bottom.resize(actCellCount, std::numeric_limits<double>::max());

    for (std::size_t cellIdx = 0; cellIdx < grid.cells.size(); ++cellIdx) {
        const RigCell& cell = grid.cells[cellIdx];
        std::size_t resultIndex = info.cellResultIndex(cellIdx);
        if (resultIndex == UNDEFINED_SIZE_T) continue;

        bool isTemporaryGrid = cell.hostGrid()->isTempGrid();

        if (computeDepth || isTemporaryGrid)
            depth[resultIndex] = std::abs(cell.center().z);

        if (computeDx || isTemporaryGrid) {
            double width = (cell.faceCenter(POS_I).x - cell.faceCenter(NEG_I).x);
            dx[resultIndex] = std::abs(width);
        }

        if (computeDy || isTemporaryGrid) {
            double height = (cell.faceCenter(POS_J).y - cell.faceCenter(NEG_J).y);
            dy[resultIndex] = std::abs(height);
        }

        if (computeDz || isTemporaryGrid) {
            double depthValue = (cell.faceCenter(POS_K).z - cell.faceCenter(NEG_K).z);
            dz[resultIndex] = std::abs(depthValue);
        }

        if (computeTops || isTemporaryGrid)
            tops[resultIndex] = std::abs(cell.faceCenter(NEG_K).z);

        if (computeBottom || isTemporaryGrid)
            bottom[resultIndex] = std::abs(cell.faceCenter(POS_K).z);
    }
}

// Explicit template instantiations
