#include <cstddef>

// Naive matrix multiplication of A[m x k] and B[k x n]
static void gemm(const float* a, const float* b,
                 bool trans_a, bool trans_b,
                 int m, int n, int k,
                 float alpha, float beta,
                 float* c)
{
    for (int i = 0; i < m; i += 2) {
        for (int j = 0; j < n; j += 2) {
            float sum00 = 0.f, sum01 = 0.f, sum10 = 0.f, sum11 = 0.f;
            for (int p = 0; p < k; ++p) {
                float av0 = trans_a ? a[p * m + i] : a[i * k + p];
                float av1 = trans_a ? a[p * m + i + 1] : a[(i + 1) * k + p];
                float bv0 = trans_b ? b[j * k + p] : b[p * n + j];
                float bv1 = trans_b ? b[(j + 1) * k + p] : b[p * n + j + 1];
                sum00 += av0 * bv0;
                sum01 += av0 * bv1;
                sum10 += av1 * bv0;
                sum11 += av1 * bv1;
            }
            c[i * n + j] = alpha * sum00 + beta * c[i * n + j];
            c[i * n + j + 1] = alpha * sum01 + beta * c[i * n + j + 1];
            c[(i + 1) * n + j] = alpha * sum10 + beta * c[(i + 1) * n + j];
            c[(i + 1) * n + j + 1] = alpha * sum11 + beta * c[(i + 1) * n + j + 1];
        }
    }
}

// Sequential batched GEMM
void gemm_batch(const float* a, const float* b,
                bool transpose_a, bool transpose_b,
                int batch_size,
                int m, int n, int k,
                float alpha, float beta,
                float* c)
{
    for (int i = 0; i < batch_size; ++i) {
        const float* a_i = a + i * m * k;
        const float* b_i = b + i * k * n;
        float* c_i = c + i * m * n;
        gemm(a_i, b_i, transpose_a, transpose_b, m, n, k, alpha, beta, c_i);
    }
}
