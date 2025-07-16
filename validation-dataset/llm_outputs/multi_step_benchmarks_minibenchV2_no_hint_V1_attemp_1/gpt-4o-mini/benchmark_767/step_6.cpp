// original.cpp
#include <cstddef>
#include <immintrin.h>

// Optimized matrix multiplication of A[m x k] and B[k x n]
static void gemm(const float* a, const float* b,
                 bool trans_a, bool trans_b,
                 int m, int n, int k,
                 float alpha, float beta,
                 float* c)
{
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            __m256 sum = _mm256_setzero_ps(); // Use AVX for sum
            int p = 0;
            for (; p <= k - 8; p += 8) {
                // Load a and b using AVX
                __m256 av = trans_a ? _mm256_loadu_ps(&a[p * m + i]) : _mm256_loadu_ps(&a[i * k + p]);
                __m256 bv = trans_b ? _mm256_loadu_ps(&b[j * k + p]) : _mm256_loadu_ps(&b[p * n + j]);
                sum = _mm256_fmadd_ps(av, bv, sum); // Fused multiply add
            }
            // Handle remaining elements
            float scalar_sum = 0.f;
            for (; p < k; ++p) {
                float av = trans_a ? a[p * m + i] : a[i * k + p];
                float bv = trans_b ? b[j * k + p] : b[p * n + j];
                scalar_sum += av * bv;
            }
            // Horizontal add for AVX sum
            sum = _mm256_hadd_ps(sum, sum);
            sum = _mm256_hadd_ps(sum, sum);
            float final_sum;
            _mm256_store_ss(&final_sum, sum);
            c[i * n + j] = alpha * (scalar_sum + final_sum) + beta * c[i * n + j];
        }
    }
}

// Sequential batched GEMM with optimized gemm
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

// Explicit template instantiations as in original file
// ... (the explicit template instantiation section is unchanged)