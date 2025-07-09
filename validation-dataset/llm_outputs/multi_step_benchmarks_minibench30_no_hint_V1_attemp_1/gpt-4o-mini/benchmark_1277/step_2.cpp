#include <cmath>
#include <unordered_map>
#include <set>
#ifdef _OPENMP
#include <omp.h>
#endif

// Mimic the expensive two-electron integral loops from Ambit.
unsigned int CalculateTwoElectronIntegrals(bool check_size_only, int size) {
    std::set<int> found_keys;
    std::unordered_map<int,double> integrals;

    if (!check_size_only) {
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
    } else {
        // Collect keys without critical sections
        #pragma omp parallel
        {
            std::set<int> local_keys;
            #pragma omp for schedule(static)
            for (int i1 = 0; i1 < size; ++i1) {
                for (int i2 = 0; i2 < size; ++i2) {
                    for (int i3 = 0; i3 < size; ++i3) {
                        for (int i4 = 0; i4 < size; ++i4) {
                            int key = (((i1 * size + i2) * size + i3) * size + i4);
                            local_keys.insert(key);
                        }
                    }
                }
            }
            #pragma omp critical(TWO_ELECTRON_SLATER)
            {
                found_keys.insert(local_keys.begin(), local_keys.end());
            }
        }
    }
    return check_size_only ? found_keys.size() : integrals.size();
}

// Explicit template instantiations (if needed)