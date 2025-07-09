#include <cmath>
#include <unordered_map>
#include <set>
#ifdef _OPENMP
#include <omp.h>
#endif

// Mimic the expensive two-electron integral loops from Ambit.
// The original version always launches an OpenMP parallel region
// even when only checking integral sizes.

unsigned int CalculateTwoElectronIntegrals(bool check_size_only, int size) {
    std::set<int> found_keys;
    std::unordered_map<int,double> integrals;

#pragma omp parallel
    {
#pragma omp for schedule(static)
        for (int i1 = 0; i1 < size; ++i1) {
            for (int i2 = 0; i2 < size; ++i2) {
                for (int i3 = 0; i3 < size; ++i3) {
                    for (int i4 = 0; i4 < size; ++i4) {
                        int key = (((i1 * size + i2) * size + i3) * size + i4);
                        if (check_size_only) {
#ifdef _OPENMP
#pragma omp critical(TWO_ELECTRON_SLATER)
#endif
                            found_keys.insert(key);
                        } else {
                            double radial = std::sin(i1 * i2 + i3 * i4) *
                                             std::sqrt(i1 + i2 + i3 + i4 + 1.0);
#ifdef _OPENMP
#pragma omp critical(TWO_ELECTRON_SLATER)
#endif
                            integrals[key] = radial;
                        }
                    }
                }
            }
        }
    }
    return check_size_only ? found_keys.size() : integrals.size();
}
