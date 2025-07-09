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

// Original pairwise traversal using two nested loops
void compute_distance_matrix(const std::vector<std::vector<double>>& data,
                             std::vector<std::vector<double>>& dist) {
    size_t n = data.size();
    #pragma omp parallel for collapse(2)
    for (size_t row = 0; row < n; ++row) {
        for (size_t col = 0; col < n; ++col) {
            if (row <= col)
                continue;
            double d = distance_func(data[row], data[col]);
            dist[row][col] = d;
            dist[col][row] = d;
        }
    }
}
