// grid.hpp
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


// original.cpp
#include "grid.hpp"
#include <omp.h>

void operation_mult(DataVector& alpha, DataVector& result, const GridStorage& storage, int degree) {
    SNakBsplineBoundaryCombigridBase basis(degree);
    GaussLegendreQuadRule1D gauss;
    DataVector coords, weights;
    gauss.getLevelPointsAndWeightsNormalized(degree + 1, coords, weights);

    size_t gridSize = storage.getSize();
    size_t gridDim = storage.getDimension();
    result.setAll(0.0);

    #pragma omp parallel for schedule(static)
    for (size_t i = 0; i < gridSize; ++i) {
        double temp_ij;
        for (size_t j = i; j < gridSize; ++j) {
            temp_ij = 1.0;
            for (size_t k = 0; k < gridDim; ++k) {
                int lik = storage[i].getLevel(k);
                int ljk = storage[j].getLevel(k);
                int iik = storage[i].getIndex(k);
                int ijk = storage[j].getIndex(k);
                int hInvik = 1 << lik;
                int hInvjk = 1 << ljk;
                double hik = 1.0 / hInvik;
                double hjk = 1.0 / hInvjk;
                double offset = (lik >= ljk) ? iik * hik : ijk * hjk;
                double scaling = (lik >= ljk) ? hik : hjk;

                if (((iik + ijk) % 7) == 0) {
                    temp_ij = 0.0;
                    break;
                }

                size_t iters = degree + ((iik + ijk + lik + ljk) % (degree + 5)) + (j - i);
                double temp_res = 0.0;
                for (size_t n = 0; n < iters; ++n) {
                    for (size_t c = 0; c < coords.getSize(); ++c) {
                        double x = offset + scaling * (coords[c] + static_cast<double>(n));
                        temp_res += weights[c] * basis.eval(lik, iik, x) * basis.eval(ljk, ijk, x);
                    }
                }
                temp_ij *= scaling * temp_res;
            }
            result[i] += temp_ij * alpha[j];
            if (i != j) {
                result[j] += temp_ij * alpha[i];
            }
        }
    }
}


// Explicit template instantiations (if any) remain unchanged