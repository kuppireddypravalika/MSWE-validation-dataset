#include <vector>
#include <cmath>
#include <omp.h>

static bool heavy_compute(const std::vector<double>& v) {
    double sum = 0.0;
    for (int it = 0; it < 200; ++it) {
        for (double x : v) sum += x * 0.00001 + it * 1e-6;
    }
    return sum > 1e9;
}

void computeCollisions(size_t num_threads,
                       const std::vector<std::vector<double>>& q,
                       std::vector<bool>& res,
                       bool stopAtFirstCollisionInConfiguration = false,
                       bool stopAtFirstCollisionInBatch = false) {
    size_t batch_size = res.size();
    if (stopAtFirstCollisionInBatch) {
        bool is_colliding = false;
        size_t i = 0;
#pragma omp parallel for schedule(static) num_threads(num_threads)
        for (i = 0; i < batch_size; ++i) {
            if (is_colliding) continue;
            bool col = heavy_compute(q[i]);
            res[i] = col;
            if (col) {
                is_colliding = true;
            }
        }
    } else {
        size_t i = 0;
#pragma omp parallel for schedule(static) num_threads(num_threads)
        for (i = 0; i < batch_size; ++i) {
            res[i] = heavy_compute(q[i]);
        }
    }
}
