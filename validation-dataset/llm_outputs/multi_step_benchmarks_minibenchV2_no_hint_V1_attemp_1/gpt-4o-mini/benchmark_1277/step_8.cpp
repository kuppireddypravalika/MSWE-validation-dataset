// original.cpp
#include <cmath>
#include <unordered_map>
#include <set>
#include <vector>
#ifdef _OPENMP
#include <omp.h>
#endif

unsigned int CalculateTwoElectronIntegrals(bool check_size_only, int size) {
    std::set<int> found_keys;
    std::unordered_map<int, double> integrals;

    if (check_size_only) {
        std::vector<bool> local_keys(size * size * size * size, false);

        #pragma omp parallel
        {
            #pragma omp for schedule(static)
            for (int i1 = 0; i1 < size; ++i1) {
                for (int i2 = 0; i2 < size; ++i2) {
                    for (int i3 = 0; i3 < size; ++i3) {
                        for (int i4 = 0; i4 < size; ++i4) {
                            int key = (((i1 * size + i2) * size + i3) * size + i4);
                            local_keys[key] = true;
                        }
                    }
                }
            }
        }

        for (int i = 0; i < local_keys.size(); ++i) {
            if (local_keys[i]) {
                found_keys.insert(i);
            }
        }
        return found_keys.size();
    } else {
        #pragma omp parallel
        {
            #pragma omp for schedule(static)
            for (int i1 = 0; i1 < size; ++i1) {
                for (int i2 = 0; i2 < size; ++i2) {
                    for (int i3 = 0; i3 < size; ++i3) {
                        for (int i4 = 0; i4 < size; ++i4) {
                            int key = (((i1 * size + i2) * size + i3) * size + i4);
                            double radial = std::sin(i1 * i2 + i3 * i4) * 
                                            std::sqrt(i1 + i2 + i3 + i4 + 1.0);
                            integrals[key] = radial;
                        }
                    }
                }
            }
        }
        return integrals.size();
    }
}

// Explicit template instantiations
// <insert necessary explicit template instantiations here>