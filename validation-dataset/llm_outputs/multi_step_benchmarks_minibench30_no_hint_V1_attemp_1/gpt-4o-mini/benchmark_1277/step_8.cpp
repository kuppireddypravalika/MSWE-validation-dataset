#include <cmath>
#include <unordered_map>
#include <set>
#include <vector>
#ifdef _OPENMP
#include <omp.h>
#endif

// Optimized version to compute two-electron integrals with enhanced performance.

unsigned int CalculateTwoElectronIntegrals(bool check_size_only, int size) {
    std::set<int> found_keys;
    std::unordered_map<int,double> integrals;

    if (check_size_only) {
        // Gather keys in parallel with local collections.
        #pragma omp parallel
        {
            std::vector<int> local_keys(size * size * size * size);
            int local_count = 0;

            #pragma omp for schedule(static)
            for (int i1 = 0; i1 < size; ++i1) {
                for (int i2 = 0; i2 < size; ++i2) {
                    for (int i3 = 0; i3 < size; ++i3) {
                        for (int i4 = 0; i4 < size; ++i4) {
                            int key = (((i1 * size + i2) * size + i3) * size + i4);
                            local_keys[local_count++] = key;
                        }
                    }
                }
            }

            // Use critical section only to collect unique keys.
            #pragma omp critical(TWO_ELECTRON_SLATER)
            {
                for (int i = 0; i < local_count; ++i) {
                    found_keys.insert(local_keys[i]);
                }
            }
        }
    } else {
        // Calculate integrals in parallel, avoiding critical sections
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
    }
    return check_size_only ? found_keys.size() : integrals.size();
}

// Explicit template instantiations (if needed)