#include <cmath>
#include <unordered_map>
#include <set>
#include <vector>
#include <omp.h>

// Mimic the expensive two-electron integral loops from Ambit.
// The original version always launches an OpenMP parallel region
// even when only checking integral sizes.

unsigned int CalculateTwoElectronIntegrals(bool check_size_only, int size) {
    std::set<int> found_keys;
    std::unordered_map<int,double> integrals;

    #pragma omp parallel
    {
        std::vector<int> local_found_keys;
        std::unordered_map<int,double> local_integrals;

        #pragma omp for schedule(static)
        for (int i1 = 0; i1 < size; ++i1) {
            for (int i2 = 0; i2 < size; ++i2) {
                for (int i3 = 0; i3 < size; ++i3) {
                    for (int i4 = 0; i4 < size; ++i4) {
                        int key = (((i1 * size + i2) * size + i3) * size + i4);
                        if (check_size_only) {
                            local_found_keys.push_back(key);
                        } else {
                            double radial = std::sin(i1 * i2 + i3 * i4) *
                                             std::sqrt(i1 + i2 + i3 + i4 + 1.0);
                            local_integrals[key] = radial;
                        }
                    }
                }
            }
        }

        #pragma omp critical
        {
            if (check_size_only) {
                found_keys.insert(local_found_keys.begin(), local_found_keys.end());
            } else {
                integrals.insert(local_integrals.begin(), local_integrals.end());
            }
        }
    }
    return check_size_only ? found_keys.size() : integrals.size();
}
