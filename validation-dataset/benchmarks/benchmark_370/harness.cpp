#include <Eigen/Dense>
#include <vector>
#include <random>
#include <chrono>
#include <iostream>
#include <string>

void compute_output_step(const std::vector<Eigen::Matrix3d>& stresses,
                         std::vector<Eigen::Vector3d>& Vi_pos,
                         std::vector<Eigen::Vector3d>& Vsiii_pos);

std::vector<Eigen::Matrix3d> generate_stresses(size_t count) {
    std::vector<Eigen::Matrix3d> data(count);
    std::mt19937 gen(42);
    std::uniform_real_distribution<double> dist(-100.0, 100.0);
    for (auto& M : data) {
        double a = dist(gen); double b = dist(gen); double c = dist(gen);
        double d = dist(gen); double e = dist(gen); double f = dist(gen);
        M << a, d, e,
             d, b, f,
             e, f, c; // symmetric tensor
    }
    return data;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    size_t num = 2000;
    int repeat = 6000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--size=",0)==0) num = std::stoul(arg.substr(7));
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
    }

    auto stresses = generate_stresses(num);
    std::vector<Eigen::Vector3d> v1, v3;

    if (mode == "correct") {
        compute_output_step(stresses, v1, v3);
        double checksum = v1[0].sum() + v3[0].sum();
        std::cout << checksum << std::endl;
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for(int i=0;i<repeat;++i) {
            compute_output_step(stresses, v1, v3);
        }
        auto t2 = clock::now();
        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms\n";
    }
    return 0;
}

