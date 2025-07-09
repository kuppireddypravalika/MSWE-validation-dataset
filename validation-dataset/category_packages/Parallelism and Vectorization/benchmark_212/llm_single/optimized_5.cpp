#include <algorithm>
#include <cmath>
#include <numeric>
#include <thread>
#include <vector>
#include <functional>
#include <execution>

constexpr int kWork = 280000; // number of inner loop iterations

void heavy_op(double &v) {
    double sum = 0.0;
    for (int i = 0; i < kWork; ++i) {
        double x = i * 0.001;
        sum += std::sin(x);
    }
    v += sum;
}

// Improved parallel_for_each using parallel execution policy
template <class Iterator, class Func>
void optimized_parallel_for_each(Iterator begin, Iterator end, Func func) {
    std::for_each(std::execution::par, begin, end, func);
}

// Driver that processes a vector and returns the sum
double process(int size) {
    std::vector<double> data(size);
    std::iota(data.begin(), data.end(), 0.0);
    optimized_parallel_for_each(data.begin(), data.end(), [](double &x) { heavy_op(x); });
    return std::accumulate(data.begin(), data.end(), 0.0);
}

// Explicit template instantiation statements
template void original_parallel_for_each(std::vector<double>::iterator, std::vector<double>::iterator, std::function<void(double&)>);
