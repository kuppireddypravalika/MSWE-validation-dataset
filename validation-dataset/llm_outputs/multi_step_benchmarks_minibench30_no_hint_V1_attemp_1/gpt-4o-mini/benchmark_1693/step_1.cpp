#include "search.hpp"
#include <omp.h>
#include <algorithm>

size_t dem_fem_search(const ParticleVector& elements) {
    const std::size_t MaxNumberOfElements = elements.size();
    std::vector<std::vector<ParticlePtr>> rResults(elements.size());
    std::vector<std::vector<double>> rResultsDistance(elements.size());

    double mGlobal_BB_LowPoint[3] = {-1.0, -1.0, -1.0};
    double mGlobal_BB_HighPoint[3] = { 1.0,  1.0,  1.0};

    #pragma omp parallel
    {
        std::vector<ParticlePtr> localResults(MaxNumberOfElements);
        std::vector<double> localResultsDistances(MaxNumberOfElements);
        std::size_t NumberOfResults;

        #pragma omp for
        for (int p = 0; p < static_cast<int>(elements.size()); ++p) {
            auto go_it = elements[p];
            bool search_particle = true;
            const double* aux_coor = go_it->coord.data();
            double Rad = go_it->radius;

            for (unsigned int i = 0; i < 3; ++i)
                search_particle &= !(aux_coor[i] < (mGlobal_BB_LowPoint[i] - Rad)) && !(aux_coor[i] > (mGlobal_BB_HighPoint[i] + Rad));

            if (search_particle) {
                NumberOfResults = 0;
                for (std::size_t j = 0; j < elements.size(); ++j) {
                    if (j == static_cast<std::size_t>(p)) continue;
                    const auto& other = elements[j];
                    double dx = aux_coor[0] - other->coord[0];
                    double dy = aux_coor[1] - other->coord[1];
                    double dz = aux_coor[2] - other->coord[2];
                    double dist2 = dx*dx + dy*dy + dz*dz;
                    if (dist2 < Rad + other->radius) {
                        if (NumberOfResults < MaxNumberOfElements) {
                            localResults[NumberOfResults] = other;
                            localResultsDistances[NumberOfResults] = dist2;
                        }
                        ++NumberOfResults;
                    }
                }

                if (NumberOfResults > 0) {
                    volatile double sink = 0.0;
                    for (int w = 0; w < go_it->workload * 20; ++w)
                        for (int k = 0; k < 20; ++k)
                            sink += std::sin(w * 0.001 + k);
                    localResultsDistances[0] += sink;

                    std::lock_guard<std::mutex> lock(mutex);
                    rResults[p].insert(rResults[p].begin(), localResults.begin(), localResults.begin() + std::min(NumberOfResults, MaxNumberOfElements));
                }
            }
        }
    }

    std::size_t total = 0;
    for (const auto& v : rResults) total += v.size();
    return total;
}

std::mutex mutex;

// Explicit template instantiations (if any)
