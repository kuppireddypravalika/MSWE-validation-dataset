#include <iostream>

extern "C" double run(size_t repeat);

int main() {
    size_t repeat = 1000;  // Number of repetitions for the benchmark
    double result = run(repeat);
    std::cout << "Benchmark result: " << result << std::endl;
    return 0;
}
