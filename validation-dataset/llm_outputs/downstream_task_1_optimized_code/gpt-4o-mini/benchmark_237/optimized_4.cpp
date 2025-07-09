#include <vector>
#include <cmath>
#include <numeric>

using Matrix = std::vector<std::vector<double>>;

// In-place LLT decomposition. Computes the Cholesky factor L of A directly in a
// single output matrix row to reduce memory allocations and copy overhead.
static void cholesky_decompose(const Matrix& A, Matrix& L) {
    int N = static_cast<int>(A.size());
    L.assign(N, std::vector<double>(N, 0.0));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j <= i; ++j) {
            double sum = 0.0;
            for (int k = 0; k < j; ++k) {
                sum += L[i][k] * L[j][k];
            }
            if (i == j) {
                L[i][j] = std::sqrt(A[i][i] - sum);
            } else {
                L[i][j] = (A[i][j] - sum) / L[j][j];
            }
        }
    }
}

// Returns the sum of all elements of the Cholesky factor of A.
double llt_workload(const Matrix& A) {
    int N = static_cast<int>(A.size());
    Matrix L(N, std::vector<double>(N, 0.0));
    cholesky_decompose(A, L);

    // Using a single loop and accumulate for sum of elements to enhance performance.
    double sum = 0.0;
    for (const auto& row : L) {
        sum += std::accumulate(row.begin(), row.end(), 0.0);
    }
    return sum;
}

// Explicit template instantiation statements (if any) would go here.