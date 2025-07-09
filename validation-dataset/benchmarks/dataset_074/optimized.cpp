#include <vector>
#include <cmath>
#include <omp.h>

static double distance_func(const std::vector<double>& a,
                            const std::vector<double>& b) {
    double d = 0.0;
    for (size_t i = 0; i < a.size(); ++i) {
        double diff = a[i] - b[i];
        d += diff * diff;
    }
    return std::sqrt(d);
}

// Optimized traversal iterates directly over the upper triangle
void compute_distance_matrix(const std::vector<std::vector<double>>& data,
                             std::vector<std::vector<double>>& dist) {
    size_t n = data.size();
    size_t m = n * (n - 1) / 2;
    #pragma omp parallel for
    for (size_t k = 0; k < m; ++k) {
        size_t row = size_t(double(n - 2) -
                            std::floor(std::sqrt(-8.0 * k + 4.0 * n * (n - 1) - 7.0) / 2.0 - 0.5));
        size_t col = size_t(k + row + 1 - n * (n - 1) / 2 +
                            (n - row) * ((n - row) - 1) / 2);
        double d = distance_func(data[row], data[col]);
        dist[row][col] = d;
        dist[col][row] = d;
    }
}
