#include <Eigen/Core>
#include <unsupported/Eigen/CXX11/Tensor>
#include <chrono>
#include <iostream>
#include <string>

template <typename T>
void logsumexp(const Eigen::Tensor<T, 2>& x, Eigen::Tensor<T, 1>& y, const int dim);

int main(int argc, char* argv[]) {
    // Default settings
    std::string mode = "perf";           // "perf" or "correct"
    int repeat = 10;
    int rows = 1024;
    int cols = 512;
    int dim = 1; // Dimension to reduce over (0 or 1)

    // Parse command-line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") {
            mode = "correct";
        }
        else if (arg == "--mode=perf") {
            mode = "perf";
        }
        else if (arg.rfind("--repeat=", 0) == 0) {
            repeat = std::stoi(arg.substr(9));
        }
        else if (arg.rfind("--size=", 0) == 0) {
            std::string sizes = arg.substr(7);
            auto pos = sizes.find('x');
            rows = std::stoi(sizes.substr(0, pos));
            cols = std::stoi(sizes.substr(pos + 1));
        }
        else if (arg.rfind("--dim=", 0) == 0) {
            dim = std::stoi(arg.substr(6));
        }
    }

    // Prepare input tensor
    Eigen::Tensor<float, 2> x(rows, cols);
    x.setRandom();
    Eigen::Tensor<float, 1> y(dim == 0 ? cols : rows);

    if (mode == "correct") {
        logsumexp(x, y, dim);
        for (int i = 0; i < y.size(); ++i) std::cout << y(i) << " ";
        std::cout << std::endl;
    } else {
        using clock = std::chrono::high_resolution_clock;

        auto t1 = clock::now();
        for (int i = 0; i < repeat; ++i) {
            logsumexp(x, y, dim);
        }
        auto t2 = clock::now();

        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }

    return 0;
}
