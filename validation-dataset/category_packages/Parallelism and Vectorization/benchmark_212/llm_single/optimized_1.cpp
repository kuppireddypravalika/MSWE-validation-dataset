#include <algorithm>
#include <cmath>
#include <numeric>
#include <thread>
#include <vector>
#include <functional>
#include <execution>

constexpr int kWork = 280000; // number of inner loop iterations

void heavy_op(double &v) {
    for (int i = 0; i < kWork; ++i) {
        double x = i * 0.001;
        v += std::sin(x);
    }
}

// Optimized parallel_for_each using parallel execution
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
#include <vector>
#include <functional>
#include <numeric>
#include <thread>
template void original_parallel_for_each(std::vector<double>::iterator, std::vector<double>::iterator, std::function<void(double &)>);
