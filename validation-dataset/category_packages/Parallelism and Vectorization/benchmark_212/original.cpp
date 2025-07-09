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
    for (int i = 0; i < kWork; ++i) {
        double x = i * 0.001;
        v += std::sin(x);
    }
}

// Original parallel_for_each using 1024 element chunks
template <class Iterator, class Func>
void original_parallel_for_each(Iterator begin, Iterator end, Func func) {
    TaskGroup tg;
    ptrdiff_t taskSize = 1024;
    while (taskSize <= std::distance(begin, end)) {
        tg.spawn([=, &func] { std::for_each(begin, begin + taskSize, func); });
        begin += taskSize;
    }
    std::for_each(begin, end, func);
    tg.sync();
}

// Driver that processes a vector and returns the sum
double process(int size) {
    std::vector<double> data(size);
    std::iota(data.begin(), data.end(), 0.0);
    original_parallel_for_each(data.begin(), data.end(), [](double &x) { heavy_op(x); });
    return std::accumulate(data.begin(), data.end(), 0.0);
}
