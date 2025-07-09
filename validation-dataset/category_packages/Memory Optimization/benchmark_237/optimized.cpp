#include <vector>
#include <cmath>

using Matrix = std::vector<std::vector<double>>;

// In-place LLT decomposition. The lower triangular factor overwrites A.
static void cholesky_inplace(Matrix& A) {
    int N = static_cast<int>(A.size());
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j <= i; ++j) {
            double sum = A[i][j];
            for (int k = 0; k < j; ++k)
                sum -= A[i][k] * A[j][k];
            if (i == j)
                A[i][j] = std::sqrt(sum);
            else
                A[i][j] = sum / A[j][j];
        }
    }
}

// Returns the sum of all elements of the Cholesky factor of A.
double llt_workload(const Matrix& A) {
    Matrix work = A;      // work matrix modified in-place
    cholesky_inplace(work);

    double sum = 0.0;
    for (const auto& row : work)
        for (double v : row)
            sum += v;
    return sum;
}
