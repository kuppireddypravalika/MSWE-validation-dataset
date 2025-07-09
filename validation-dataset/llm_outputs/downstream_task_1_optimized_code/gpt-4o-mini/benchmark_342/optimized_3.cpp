#include <vector>
#include <cmath>
#include <omp.h>

static bool heavy_compute(const std::vector<double>& v) {
    // Reorganize loops for better optimization
    double sum = 0.0;
    for (int it = 0; it < 200; ++it) {
        for (size_t j = 0; j < v.size(); ++j) {
            sum += v[j] * 0.00001 + it * 1e-6;
        }
    }
    return sum > 1e9; // always false, but prevents optimization
}

void computeCollisions(size_t num_threads,
                       const std::vector<std::vector<double>>& q,
                       std::vector<bool>& res,
                       bool stopAtFirstCollisionInConfiguration = false,
                       bool stopAtFirstCollisionInBatch = false) {
    size_t batch_size = res.size();
    bool is_colliding = false;

    #pragma omp parallel
    {
        bool local_collision = false;
        #pragma omp for schedule(dynamic) nowait
        for (size_t i = 0; i < batch_size; ++i) {
            if (stopAtFirstCollisionInBatch && local_collision) {
                continue;
            }
            bool col = heavy_compute(q[i]);
            res[i] = col;
            if (col) {
                local_collision = true;
                #pragma omp atomic write
                is_colliding = true;
            }
        }
    }
}

// Explicit template instantiation statements go here.