#include <algorithm>
#include <cmath>
#include <numeric>
#include <thread>
#include <vector>
#include <functional>

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
    double sin_sum = 0.0;
    for (int i = 0; i < kWork; ++i) {
        double x = i * 0.001;
        sin_sum += std::sin(x);
    }
    v += sin_sum;
}

// Optimized parallel_for_each using std::thread and better chunk handling
template <class Iterator, class Func>
void optimized_parallel_for_each(Iterator begin, Iterator end, Func func) {
    ptrdiff_t size = std::distance(begin, end);
    int num_threads = std::thread::hardware_concurrency();
    TaskGroup tg;
    ptrdiff_t taskSize = (size + num_threads - 1) / num_threads;
    while (begin < end) {
        Iterator chunk_end = std::min(begin + taskSize, end);
        tg.spawn([=, &func] { std::for_each(begin, chunk_end, func); });
        begin = chunk_end;
    }
    tg.sync();
}

// Driver that processes a vector and returns the sum
double process(int size) {
    std::vector<double> data(size);
    std::iota(data.begin(), data.end(), 0.0);
    optimized_parallel_for_each(data.begin(), data.end(), [](double &x) { heavy_op(x); });
    return std::accumulate(data.begin(), data.end(), 0.0);
}

// Explicit template instantiation
template void optimized_parallel_for_each<std::vector<double>::iterator, std::function<void(double&)>>(std::vector<double>::iterator, std::vector<double>::iterator, std::function<void(double&)>);