#include <vector>
#include <cmath>
#include <numeric>

using Matrix = std::vector<std::vector<double>>;

// In-place LLT decomposition. Computes the Cholesky factor L of A
static void cholesky_in_place(Matrix& A) {
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
    // Create a separate working copy of A to preserve the original data
    Matrix work = A;
    // Perform in-place Cholesky factorization
    cholesky_in_place(work);

    // Calculate the sum of the elements in the lower triangular matrix (work)
    double sum = 0.0;
    for (int i = 0; i < work.size(); ++i) {
        for (int j = 0; j <= i; ++j) {
            sum += work[i][j];
        }
    }
    return sum;
}