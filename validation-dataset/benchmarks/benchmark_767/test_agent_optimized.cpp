#include <iostream>
#include <vector>
#include <chrono>
#include "agent_optimized.cpp"

void fill_matrix(std::vector<float>& matrix, int rows, int cols) {
    for (int i = 0; i < rows * cols; ++i) {
        matrix[i] = static_cast<float>(rand()) / RAND_MAX;
    }
}

int main() {
    int batch_size = 10;
    int m = 512, n = 512, k = 512;
    float alpha = 1.0f, beta = 0.0f;

    std::vector<float> a(batch_size * m * k);
    std::vector<float> b(batch_size * k * n);
    std::vector<float> c(batch_size * m * n);

    fill_matrix(a, batch_size * m, k);
    fill_matrix(b, batch_size * k, n);
    fill_matrix(c, batch_size * m, n);

    auto start = std::chrono::high_resolution_clock::now();
    gemm_batch(a.data(), b.data(), false, false, batch_size, m, n, k, alpha, beta, c.data());
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Elapsed time: " << elapsed.count() << " seconds" << std::endl;

    return 0;
}
