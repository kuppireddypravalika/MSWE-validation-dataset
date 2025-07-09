#include <algorithm>
#include <cmath>
#include <numeric>
#include <thread>
#include <vector>
#include <functional>
#include <execution> // Include for parallel execution

// Simple TaskGroup spawning one thread per task
class TaskGroup {
    std::vector<std::thread> threads;
public:
    void spawn(std::function<void()> f) { threads.emplace_back(std::move(f)); }
    void sync() {
        for (auto &t : threads) t.join();
        threads.clear();
    }
    ~TaskGroup() { sync(); }
};

constexpr int kWork = 280000; // number of inner loop iterations

void heavy_op(double &v) {
    for (int i = 0; i < kWork; ++i) {
        double x = i * 0.001;
        v += std::sin(x);
    }
}

// Optimized parallel_for_each using parallel execution policy
template <class Iterator, class Func>
void optimized_parallel_for_each(Iterator begin, Iterator end, Func func) {
    std::for_each(std::execution::par, begin, end, func);
}

// Driver that processes a vector and returns the sum
double process(int size) {
    std::vector<double> data(size);
    std::iota(data.begin(), data.end(), 0.0);
    optimized_parallel_for_each(data.begin(), data.end(), [](double &x) { heavy_op(x); });
    return std::reduce(data.begin(), data.end(), 0.0);
}

// Original template instantiation statements (if any)
