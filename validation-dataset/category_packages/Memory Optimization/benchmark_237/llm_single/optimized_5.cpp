#include <vector>
#include <cmath>
#include <iostream>

using Matrix = std::vector<std::vector<double>>;

// In-place LLT decomposition to eliminate unnecessary copies and improve performance.
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

// Returns the sum of all elements of the Cholesky factor of A in-place.
double llt_workload(const Matrix& A) {
    Matrix work = A;          // Create a copy of A to work with
    cholesky_in_place(work);  // perform LLT decomposition in-place

    double sum = 0.0;
    for (const auto& row : work)
        for (double v : row)
            sum += v;
    return sum;
}

// Explicit template instantiation statements
// None provided in the original code.
