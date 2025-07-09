// optimized.cpp
#include <vector>
#include <cmath>

using Matrix = std::vector<std::vector<double>>;

// Out-of-place LLT decomposition. Computes the Cholesky factor L of A
// and stores it in a separate matrix which is then copied back.
static void cholesky_copy(const Matrix& A, Matrix& L) {
    int N = static_cast<int>(A.size());
    L.resize(N, std::vector<double>(N, 0.0));
    for (int i = 0; i < N; ++i) {
        double sum;
        for (int j = 0; j <= i; ++j) {
            sum = 0.0;
            for (int k = 0; k < j; ++k)
                sum += L[i][k] * L[j][k];
            if (i == j)
                L[i][j] = std::sqrt(A[i][i] - sum);
            else
                L[i][j] = (A[i][j] - sum) / L[j][j];
        }
    }
}

// Returns the sum of all elements of the Cholesky factor of A.
double llt_workload(const Matrix& A) {
    int N = static_cast<int>(A.size());
    Matrix L(N, std::vector<double>(N, 0.0));
    cholesky_copy(A, L);

    double sum = 0.0;
    for (int i = 0; i < N; ++i)
        for (double v : L[i])
            sum += v;
    return sum;
}

// Explicit template instantiation if necessary (with the given code)
// Template instantiation statements unchanged
