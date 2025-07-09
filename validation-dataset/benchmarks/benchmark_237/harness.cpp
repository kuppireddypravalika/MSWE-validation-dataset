#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using Matrix = std::vector<std::vector<double>>;

double llt_workload(const Matrix& A);

Matrix make_spd_matrix(int n) {
    Matrix A(n, std::vector<double>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j <= i; ++j) {
            double val = (i == j ? n : 1.0) + std::abs(i - j);
            A[i][j] = val;
            A[j][i] = val;
        }
    }
    return A;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int size = 800;
    int repeat = 110;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--size=", 0) == 0)
            size = std::stoi(arg.substr(7));
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stoi(arg.substr(9));
    }

    Matrix A = make_spd_matrix(size);

    if (mode == "correct") {
        double val = llt_workload(A);
        std::cout << val << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int r = 0; r < repeat; ++r) {
            llt_workload(A);
        }
        auto t2 = clock::now();

        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
    return 0;
}
