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

    bool computeDepth=false, computeDx=false, computeDy=false,
         computeDz=false, computeTops=false, computeBottom=false;

    auto ensure = [&](std::vector<double>& vec, bool& flag){
        if (vec.size() < actCellCount) {
            vec.resize(actCellCount, std::numeric_limits<double>::max());
            flag = true;
        }
    };

    ensure(depth, computeDepth);
    ensure(dx, computeDx);
    ensure(dy, computeDy);
    ensure(dz, computeDz);
    ensure(tops, computeTops);
    ensure(bottom, computeBottom);

    for (std::size_t cellIdx = 0; cellIdx < grid.cells.size(); ++cellIdx) {
        const RigCell& cell = grid.cells[cellIdx];
        std::size_t resultIndex = info.cellResultIndex(cellIdx);
        if (resultIndex == UNDEFINED_SIZE_T) continue;

        bool isTemporaryGrid = cell.hostGrid()->isTempGrid();

        if (computeDepth || isTemporaryGrid)
            depth[resultIndex] = std::abs(cell.center().z);

        if (computeDx || isTemporaryGrid) {
            Vec3 w = cell.faceCenter(NEG_I) - cell.faceCenter(POS_I);
            dx[resultIndex] = w.length();
        }

        if (computeDy || isTemporaryGrid) {
            Vec3 w = cell.faceCenter(NEG_J) - cell.faceCenter(POS_J);
            dy[resultIndex] = w.length();
        }

        if (computeDz || isTemporaryGrid) {
            Vec3 w = cell.faceCenter(NEG_K) - cell.faceCenter(POS_K);
            dz[resultIndex] = w.length();
        }

        if (computeTops || isTemporaryGrid)
            tops[resultIndex] = std::abs(cell.faceCenter(NEG_K).z);

        if (computeBottom || isTemporaryGrid)
            bottom[resultIndex] = std::abs(cell.faceCenter(POS_K).z);
    }
}

