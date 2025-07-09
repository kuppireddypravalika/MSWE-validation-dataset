#include <iostream>
#include <vector>
#include <chrono>
#include <string>
#include <random>

void silu_double_grad(const std::vector<float>& x,
                      const std::vector<float>& grad_x_grad,
                      std::vector<float>& grad_out_grad,
                      std::vector<float>& grad_x);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int repeat = 1000;
    size_t size = 1000000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") {
            mode = "correct";
        } else if (arg == "--mode=perf") {
            mode = "perf";
        } else if (arg.rfind("--repeat=", 0) == 0) {
            repeat = std::stoi(arg.substr(9));
        } else if (arg.rfind("--size=", 0) == 0) {
            size = std::stoul(arg.substr(7));
        }
    }

    std::vector<float> x(size);
    std::vector<float> grad_x_grad(size);
    std::vector<float> grad_out_grad;
    std::vector<float> grad_x;

    std::mt19937 gen(42);
    std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
    for (size_t i = 0; i < size; ++i) {
        x[i] = dist(gen);
        grad_x_grad[i] = dist(gen);
    }

    if (mode == "correct") {
        silu_double_grad(x, grad_x_grad, grad_out_grad, grad_x);
        for (size_t i = 0; i < grad_out_grad.size(); ++i) {
            std::cout << grad_out_grad[i] << " ";
        }
        std::cout << std::endl;
    } else {
        using clock = std::chrono::high_resolution_clock;

        auto t1 = clock::now();
        for (int i = 0; i < repeat; ++i) {
            silu_double_grad(x, grad_x_grad, grad_out_grad, grad_x);
        }
        auto t2 = clock::now();

        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }

    return 0;
}

