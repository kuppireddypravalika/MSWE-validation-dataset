// cell.hpp
#pragma once
#include <vector>
#include <cmath>
#include <cstddef>

struct Vec3 {
    double x, y, z;
    Vec3(double xx=0,double yy=0,double zz=0):x(xx),y(yy),z(zz){};
    Vec3 operator-(const Vec3& o) const { return Vec3{x-o.x, y-o.y, z-o.z}; }
    double length() const { return std::sqrt(x*x + y*y + z*z); }
};

enum FaceType { NEG_I, POS_I, NEG_J, POS_J, NEG_K, POS_K };

struct Grid {
    bool temporary;
    bool isTempGrid() const { return temporary; }
};

struct RigCell {
    Vec3 c;
    Grid* g;
    Vec3 faces[6];
    RigCell(Grid* grid, const Vec3& center):c(center),g(grid){
        faces[NEG_I] = Vec3(center.x - 0.5, center.y, center.z);
        faces[POS_I] = Vec3(center.x + 0.5, center.y, center.z);
        faces[NEG_J] = Vec3(center.x, center.y - 0.5, center.z);
        faces[POS_J] = Vec3(center.x, center.y + 0.5, center.z);
        faces[NEG_K] = Vec3(center.x, center.y, center.z - 0.5);
        faces[POS_K] = Vec3(center.x, center.y, center.z + 0.5);
    }
    const Vec3& center() const { return c; }
    const Vec3& faceCenter(FaceType f) const { return faces[f]; }
    Grid* hostGrid() const { return g; }
};

struct MainGrid { std::vector<RigCell> cells; };

static constexpr std::size_t UNDEFINED_SIZE_T = static_cast<std::size_t>(-1);

struct ActiveCellInfo {
    std::vector<std::size_t> mapping; // cell index -> result index
    std::size_t reservoirActiveCellCount() const { return mapping.size(); }
    std::size_t cellResultIndex(std::size_t idx) const {
        return idx < mapping.size() ? mapping[idx] : UNDEFINED_SIZE_T;
    }
};

// original.cpp
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
