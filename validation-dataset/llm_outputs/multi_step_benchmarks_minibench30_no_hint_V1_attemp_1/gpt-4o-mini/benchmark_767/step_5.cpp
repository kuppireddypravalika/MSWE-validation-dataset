#include <cstddef>
#include <immintrin.h>

// Optimized matrix multiplication of A[m x k] and B[k x n] using SIMD
static void gemm(const float* a, const float* b,
                 bool trans_a, bool trans_b,
                 int m, int n, int k,
                 float alpha, float beta,
                 float* c)
{
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            __m256 sum_vec = _mm256_setzero_ps();
            int p;
            for (p = 0; p + 8 <= k; p += 8) {
                __m256 av = trans_a ? _mm256_loadu_ps(&a[p * m + i]) : _mm256_loadu_ps(&a[i * k + p]);
                __m256 bv = trans_b ? _mm256_loadu_ps(&b[j * k + p]) : _mm256_loadu_ps(&b[p * n + j]);
                sum_vec = _mm256_fmadd_ps(av, bv, sum_vec);
            }
            // Handle remaining elements
            float sum_tail = 0.0f;
            for (; p < k; p++) {
                float av = trans_a ? a[p * m + i] : a[i * k + p];
                float bv = trans_b ? b[j * k + p] : b[p * n + j];
                sum_tail += av * bv;
            }
            float sum[8];
            _mm256_storeu_ps(sum, sum_vec);
            float final_sum = sum[0] + sum[1] + sum[2] + sum[3] + sum[4] + sum[5] + sum[6] + sum[7] + sum_tail;
            c[i * n + j] = alpha * final_sum + beta * c[i * n + j];
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

// Explicit template instantiations