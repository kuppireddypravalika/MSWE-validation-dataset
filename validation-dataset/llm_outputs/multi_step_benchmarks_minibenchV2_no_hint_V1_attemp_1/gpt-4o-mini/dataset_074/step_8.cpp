// original.cpp
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

// Optimized pairwise traversal using two nested loops
void compute_distance_matrix(const std::vector<std::vector<double>>& data,
                             std::vector<std::vector<double>>& dist) {
    size_t n = data.size();
    #pragma omp parallel for
    for (size_t row = 0; row < n; ++row) {
        for (size_t col = row + 1; col < n; ++col) {
            double d = distance_func(data[row], data[col]);
            dist[row][col] = d;
            dist[col][row] = d;
        }
    }
}

// Explicit instantiations
#include <vector>
#include <cmath>
#include <omp.h>

static double distance_func(const std::vector<double>& a,
                            const std::vector<double>& b);

void compute_distance_matrix(const std::vector<std::vector<double>>& data,
                             std::vector<std::vector<double>>& dist);