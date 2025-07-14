#include "cell.hpp"
#include <vector>
#include <cmath>
#include <limits>
#include <omp.h>

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
        if (vec.empty()) {
            vec.reserve(actCellCount);
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

    bool isTemporaryGrid = grid.cells.empty() ? false : grid.cells[0].hostGrid()->isTempGrid();

    #pragma omp parallel for
    for (std::size_t cellIdx = 0; cellIdx < grid.cells.size(); ++cellIdx) {
        const RigCell& cell = grid.cells[cellIdx];
        std::size_t resultIndex = info.cellResultIndex(cellIdx);
        if (resultIndex == UNDEFINED_SIZE_T) continue;

        const Vec3 faceCenterNegI = cell.faceCenter(NEG_I);
        const Vec3 faceCenterPosI = cell.faceCenter(POS_I);
        const Vec3 faceCenterNegJ = cell.faceCenter(NEG_J);
        const Vec3 faceCenterPosJ = cell.faceCenter(POS_J);
        const Vec3 faceCenterNegK = cell.faceCenter(NEG_K);
        const Vec3 faceCenterPosK = cell.faceCenter(POS_K);

        if (computeDepth || isTemporaryGrid)
            depth[resultIndex] = std::abs(cell.center().z);

        if (computeDx || isTemporaryGrid) {
            Vec3 w = faceCenterNegI - faceCenterPosI;
            dx[resultIndex] = w.length();
        }

        if (computeDy || isTemporaryGrid) {
            Vec3 w = faceCenterNegJ - faceCenterPosJ;
            dy[resultIndex] = w.length();
        }

        if (computeDz || isTemporaryGrid) {
            Vec3 w = faceCenterNegK - faceCenterPosK;
            dz[resultIndex] = w.length();
        }

        if (computeTops || isTemporaryGrid)
            tops[resultIndex] = std::abs(faceCenterNegK.z);

        if (computeBottom || isTemporaryGrid)
            bottom[resultIndex] = std::abs(faceCenterPosK.z);
    }
}
