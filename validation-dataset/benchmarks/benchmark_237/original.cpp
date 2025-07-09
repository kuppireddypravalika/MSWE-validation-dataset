#include <vector>
#include <cmath>

using Matrix = std::vector<std::vector<double>>;

// Out-of-place LLT decomposition. Computes the Cholesky factor L of A
// and stores it in a separate matrix which is then copied back.
static void cholesky_copy(const Matrix& A, Matrix& L) {
    int N = static_cast<int>(A.size());
    L.assign(N, std::vector<double>(N, 0.0));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j <= i; ++j) {
            double sum = 0.0;
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
    Matrix work = A;          // simulate value_of_rec copy
    Matrix L;                 // factor stored separately
    cholesky_copy(work, L);   // compute LLT

    // copy result back (as in L_A = L_factor.matrixL())
    work = L;

    double sum = 0.0;
    for (const auto& row : work)
        for (double v : row)
            sum += v;
    return sum;
}
