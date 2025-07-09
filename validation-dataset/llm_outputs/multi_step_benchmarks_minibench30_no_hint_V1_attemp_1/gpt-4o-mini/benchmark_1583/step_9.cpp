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

    auto ensure = [&](std::vector<double>& vec) {
        if (vec.size() < actCellCount) {
            vec.resize(actCellCount, std::numeric_limits<double>::max());
        }
    };

    ensure(depth);
    ensure(dx);
    ensure(dy);
    ensure(dz);
    ensure(tops);
    ensure(bottom);

    for (std::size_t cellIdx = 0; cellIdx < grid.cells.size(); ++cellIdx) {
        const RigCell& cell = grid.cells[cellIdx];
        std::size_t resultIndex = info.cellResultIndex(cellIdx);
        if (resultIndex == UNDEFINED_SIZE_T) continue;

        const bool isTemporaryGrid = cell.hostGrid()->isTempGrid();
        const Vec3& center = cell.center();
        const Vec3& faceNegK = cell.faceCenter(NEG_K);
        const Vec3& facePosK = cell.faceCenter(POS_K);
        const Vec3& faceNegI = cell.faceCenter(NEG_I);
        const Vec3& facePosI = cell.faceCenter(POS_I);
        const Vec3& faceNegJ = cell.faceCenter(NEG_J);
        const Vec3& facePosJ = cell.faceCenter(POS_J);

        if (isTemporaryGrid) {
            depth[resultIndex] = std::abs(center.z);
            tops[resultIndex] = std::abs(faceNegK.z);
            bottom[resultIndex] = std::abs(facePosK.z);
        }

        if (dx.size() <= resultIndex || isTemporaryGrid) {
            dx[resultIndex] = (faceNegI - facePosI).length();
        }
        if (dy.size() <= resultIndex || isTemporaryGrid) {
            dy[resultIndex] = (faceNegJ - facePosJ).length();
        }
        if (dz.size() <= resultIndex || isTemporaryGrid) {
            dz[resultIndex] = (faceNegK - facePosK).length();
        }
    }
}

// Explicit template instantiations (if any)