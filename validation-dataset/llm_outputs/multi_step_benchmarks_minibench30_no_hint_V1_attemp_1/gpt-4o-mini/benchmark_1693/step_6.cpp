// original.cpp
#include "search.hpp"
#include <omp.h>
#include <vector>
#include <algorithm>
#include <cmath>

// Default static scheduling
size_t dem_fem_search(const ParticleVector& elements) {
    const std::size_t MaxNumberOfElements = elements.size();
    std::vector<std::vector<ParticlePtr>> rResults(elements.size());

    double mGlobal_BB_LowPoint[3] = {-1.0, -1.0, -1.0};
    double mGlobal_BB_HighPoint[3] = { 1.0,  1.0,  1.0};

    #pragma omp parallel
    {
        std::vector<ParticlePtr> localResults(MaxNumberOfElements);
        std::size_t NumberOfResults;

        #pragma omp for
        for (int p = 0; p < static_cast<int>(elements.size()); ++p) {
            auto go_it = elements[p];
            const double* aux_coor = go_it->coord.data();
            double Rad = go_it->radius;

            bool search_particle = true;
            for (unsigned int i = 0; i < 3; ++i)
                search_particle &= (aux_coor[i] >= (mGlobal_BB_LowPoint[i] - Rad)) && (aux_coor[i] <= (mGlobal_BB_HighPoint[i] + Rad));

            if (search_particle) {
                NumberOfResults = 0;
                for (std::size_t j = 0; j < elements.size(); ++j) {
                    if (j == static_cast<std::size_t>(p)) continue;
                    const auto& other = elements[j];
                    double dx = aux_coor[0] - other->coord[0];
                    double dy = aux_coor[1] - other->coord[1];
                    double dz = aux_coor[2] - other->coord[2];
                    double dist2 = dx * dx + dy * dy + dz * dz;

                    if (dist2 < (Rad + other->radius) && NumberOfResults < MaxNumberOfElements) {
                        localResults[NumberOfResults] = other;
                        NumberOfResults++;
                    }
                }

                if (NumberOfResults > 0) {
                    for (int w = 0; w < go_it->workload * 20; ++w) {
                        for (int k = 0; k < 20; ++k) {
                            volatile double sink = std::sin(w * 0.001 + k);
                        }
                    }
                    rResults[p].insert(rResults[p].begin(), localResults.begin(), localResults.begin() + NumberOfResults);
                }
            }
        }
    }

    std::size_t total = 0;
    for (const auto& v : rResults) total += v.size();
    return total;
}

// Explicit template instantiations
size_t dem_fem_search(const ParticleVector& elements);