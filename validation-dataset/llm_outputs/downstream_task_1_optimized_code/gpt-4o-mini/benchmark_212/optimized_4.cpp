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

// Optimized parallel_for_each using C++ standard library parallel execution policies
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
// (None required for this optimization, but should remain commented out if needed in the future)