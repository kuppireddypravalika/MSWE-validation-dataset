#include <vector>
#include <cmath>
#include <omp.h>

static double distance_func(const std::vector<double>& a,
                            const std::vector<double>& b) {
    double d = 0.0;
    size_t size = a.size();
    #pragma omp simd reduction(+:d)
    for (size_t i = 0; i < size; ++i) {
        double diff = a[i] - b[i];
        d += diff * diff;
    }
    return std::sqrt(d);
}

void compute_distance_matrix(const std::vector<std::vector<double>>& data,
                             std::vector<std::vector<double>>& dist) {
    size_t n = data.size();
    #pragma omp parallel for
    for (size_t row = 0; row < n; ++row) {
        std::vector<double> temp_dist(n);
        for (size_t col = row + 1; col < n; ++col) {
            double d = distance_func(data[row], data[col]);
            temp_dist[col] = d;
        }
        #pragma omp critical
        {
            for (size_t col = row + 1; col < n; ++col) {
                dist[row][col] = temp_dist[col];
                dist[col][row] = temp_dist[col];
            }
        }
    }
}

// Explicit template instantiations