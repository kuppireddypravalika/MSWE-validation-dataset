// original.cpp
#include "search.hpp"
#include <omp.h>
#include <vector>
#include <cmath>
#include <algorithm>

// Default static scheduling
size_t dem_fem_search(const ParticleVector& elements) {
    const std::size_t MaxNumberOfElements = elements.size();
    std::vector<std::vector<ParticlePtr>> rResults(MaxNumberOfElements);

    double mGlobal_BB_LowPoint[3] = {-1.0, -1.0, -1.0};
    double mGlobal_BB_HighPoint[3] = { 1.0,  1.0,  1.0};

    #pragma omp parallel
    {
        std::vector<ParticlePtr> localResults(MaxNumberOfElements);

        #pragma omp for
        for (std::size_t p = 0; p < MaxNumberOfElements; ++p) {
            const auto& go_it = elements[p];
            const double* aux_coor = go_it->coord.data();
            double Rad = go_it->radius;

            // Check if the particle is within the bounding box
            bool inBounds = true;
            for (int i = 0; i < 3; ++i) {
                inBounds &= (aux_coor[i] >= (mGlobal_BB_LowPoint[i] - Rad)) && (aux_coor[i] <= (mGlobal_BB_HighPoint[i] + Rad));
            }

            if (!inBounds) continue;
            std::size_t NumberOfResults = 0;

            for (std::size_t j = 0; j < MaxNumberOfElements; ++j) {
                if (j != p) {
                    const auto& other = elements[j];
                    double dx = aux_coor[0] - other->coord[0];
                    double dy = aux_coor[1] - other->coord[1];
                    double dz = aux_coor[2] - other->coord[2];
                    double dist2 = dx * dx + dy * dy + dz * dz;
                    if (dist2 < (Rad + other->radius) * (Rad + other->radius)) {
                        if (NumberOfResults < MaxNumberOfElements) {
                            localResults[NumberOfResults] = other;
                        }
                        ++NumberOfResults;
                    }
                }
            }

            double sink = 0.0;
            for (int w = 0; w < go_it->workload * 20; ++w) {
                for (int k = 0; k < 20; ++k) {
                    sink += std::sin(w * 0.001 + k);
                }
            }

            rResults[p].insert(rResults[p].end(), localResults.begin(), localResults.begin() + NumberOfResults);
        }
    }

    std::size_t total = 0;
    for (const auto& v : rResults) total += v.size();
    return total;
}

// Explicit template instantiations
size_t dem_fem_search(const ParticleVector& elements);