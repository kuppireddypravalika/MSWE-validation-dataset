#pragma once
#include <vector>
#include <algorithm>
#include <cmath>

struct DataVector {
    std::vector<double> data;
    DataVector(size_t n = 0) : data(n) {}
    size_t getSize() const { return data.size(); }
    void resize(size_t n) { data.resize(n); }
    void setAll(double v) { std::fill(data.begin(), data.end(), v); }
    double& operator[](size_t i) { return data[i]; }
    const double& operator[](size_t i) const { return data[i]; }
};

struct GridPoint {
    std::vector<int> level;
    std::vector<int> index;
    GridPoint(size_t dim = 0) : level(dim), index(dim) {}
    int getLevel(size_t d) const { return level[d]; }
    int getIndex(size_t d) const { return index[d]; }
};

struct GridStorage {
    size_t dim;
    std::vector<GridPoint> points;
    GridStorage(size_t size = 0, size_t dim_ = 1) : dim(dim_), points(size, GridPoint(dim_)) {}
    size_t getSize() const { return points.size(); }
    size_t getDimension() const { return dim; }
    GridPoint& operator[](size_t i) { return points[i]; }
    const GridPoint& operator[](size_t i) const { return points[i]; }
};

struct SNakBsplineBoundaryCombigridBase {
    int degree;
    explicit SNakBsplineBoundaryCombigridBase(int p) : degree(p) {}
    double eval(int level, int index, double x) const {
        double t = std::fabs((1.0 * (1 << level)) * x - index);
        double support = degree + 1;
        if (t >= support) return 0.0;
        double val = 1.0 - t / support;
        for (int i = 1; i < degree; ++i) val *= val;
        return val;
    }
};

struct GaussLegendreQuadRule1D {
    void getLevelPointsAndWeightsNormalized(size_t order, DataVector& coords, DataVector& weights) {
        coords.resize(order);
        weights.resize(order);
        for (size_t i = 0; i < order; ++i) {
            coords[i] = (i + 0.5) / static_cast<double>(order);
            weights[i] = 1.0 / static_cast<double>(order);
        }
    }
};
