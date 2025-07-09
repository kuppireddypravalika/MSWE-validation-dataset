#include <algorithm>
#include <cmath>
#include <numeric>
#include <thread>
#include <vector>
#include <functional>

constexpr int kWork = 280000; // same workload as original

void heavy_op(double &v) {
    for (int i = 0; i < kWork; ++i) {
        double x = i * 0.001;
        v += std::sin(x);
    }
}

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

// Optimized parallel_for_each spawning one task per element
template <class Iterator, class Func>
void optimized_parallel_for_each(Iterator begin, Iterator end, Func func) {
    TaskGroup tg;
    for (; begin != end; ++begin)
        tg.spawn([=, &func] { func(*begin); });
    tg.sync();
}

double process(int size) {
    std::vector<double> data(size);
    std::iota(data.begin(), data.end(), 0.0);
    optimized_parallel_for_each(data.begin(), data.end(), [](double &x) { heavy_op(x); });
    return std::accumulate(data.begin(), data.end(), 0.0);
}
