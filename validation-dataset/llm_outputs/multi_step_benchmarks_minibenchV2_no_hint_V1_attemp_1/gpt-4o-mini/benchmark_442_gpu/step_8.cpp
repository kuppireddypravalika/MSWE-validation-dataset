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
        double alpha_i = alpha[i]; // Cache alpha[i] for thread efficiency
        for (size_t j = i; j < gridSize; ++j) {
            double temp_ij = 1.0;
            double alpha_j = alpha[j]; // Cache alpha[j] for thread efficiency
            for (size_t k = 0; k < gridDim; ++k) {
                int lik = storage[i].getLevel(k);
                int ljk = storage[j].getLevel(k);
                int iik = storage[i].getIndex(k);
                int ijk = storage[j].getIndex(k);
                int hInvik = 1 << lik;
                int hInvjk = 1 << ljk;
                double hik = 1.0 / (double)hInvik;
                double hjk = 1.0 / (double)hInvjk;
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
                        double x = offset + scaling * (coords[c] + (double)n);
                        temp_res += weights[c] * basis.eval(lik, iik, x) * basis.eval(ljk, ijk, x);
                    }
                }
                temp_ij *= scaling * temp_res;
            }
            result[i] += temp_ij * alpha_j; // Use cached alpha[j]
            if (i != j) {
                result[j] += temp_ij * alpha_i; // Use cached alpha[i]
            }
        }
    }
}

// Explicit template instantiations
// template instantiations here if any.