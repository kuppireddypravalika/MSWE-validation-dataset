#include <vector>
#include <random>
#include <chrono>
#include <iostream>
#include <string>

void gemm_batch(const float* a, const float* b,
                bool transpose_a, bool transpose_b,
                int batch_size, int m, int n, int k,
                float alpha, float beta, float* c);

int main(int argc, char* argv[])
{
    std::string mode = "perf";
    int batch = 32;
    int m = 128, n = 128, k = 128;
    int repeat = 120;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
    }

    std::vector<float> A(batch * m * k);
    std::vector<float> B(batch * k * n);
    std::vector<float> C(batch * m * n);
    std::mt19937 gen(42);
    std::uniform_real_distribution<float> dist(0.f,1.f);
    for (float& v : A) v = dist(gen);
    for (float& v : B) v = dist(gen);

    if (mode == "correct") {
        gemm_batch(A.data(), B.data(), false, false,
                   batch, m, n, k, 1.f, 0.f, C.data());
        double sum = 0.0;
        for (float v : C) sum += v;
        std::cout << sum << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int r = 0; r < repeat; ++r) {
            gemm_batch(A.data(), B.data(), false, false,
                       batch, m, n, k, 1.f, 0.f, C.data());
        }
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
    return 0;
}
