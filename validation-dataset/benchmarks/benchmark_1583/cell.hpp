#pragma once
#include <vector>
#include <cmath>
#include <cstddef>

struct Vec3 {
    double x, y, z;
    Vec3(double xx=0,double yy=0,double zz=0):x(xx),y(yy),z(zz){}
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

