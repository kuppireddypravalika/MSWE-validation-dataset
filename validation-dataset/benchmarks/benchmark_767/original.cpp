#include <cstddef>

// Naive matrix multiplication of A[m x k] and B[k x n]
static void gemm(const float* a, const float* b,
                 bool trans_a, bool trans_b,
                 int m, int n, int k,
                 float alpha, float beta,
                 float* c)
{
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            float sum = 0.f;
            for (int p = 0; p < k; ++p) {
                float av = trans_a ? a[p * m + i] : a[i * k + p];
                float bv = trans_b ? b[j * k + p] : b[p * n + j];
                sum += av * bv;
            }
            c[i * n + j] = alpha * sum + beta * c[i * n + j];
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
