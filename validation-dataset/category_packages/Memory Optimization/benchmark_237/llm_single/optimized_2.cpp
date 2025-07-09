#include <vector>
#include <cmath>
#include <numeric>

using Matrix = std::vector<std::vector<double>>;

// In-place LLT decomposition. Computes the Cholesky factor L of A
// and stores it directly in the second part of the matrix A itself.
static void cholesky_inplace(Matrix& A) {
    int N = static_cast<int>(A.size());
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j <= i; ++j) {
            double sum = 0.0;
            for (int k = 0; k < j; ++k)
                sum += A[i][k] * A[j][k];
            if (i == j)
                A[i][j] = std::sqrt(A[i][i] - sum);
            else
                A[i][j] = (A[i][j] - sum) / A[j][j];
        }
    }
}

// Returns the sum of all elements of the Cholesky factor of A.
double llt_workload(const Matrix& A) {
    Matrix work = A;      // create a copy of A
    cholesky_inplace(work); // compute LLT in-place

    // compute the sum of the Cholesky factor directly
    double sum = 0.0;
    for (const auto& row : work)
        sum += std::accumulate(row.begin(), row.end(), 0.0);
    return sum;
}

// Explicit template instantiations
