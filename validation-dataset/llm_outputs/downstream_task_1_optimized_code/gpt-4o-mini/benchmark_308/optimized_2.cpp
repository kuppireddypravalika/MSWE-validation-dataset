#include <cmath>
#include <array>
#include <cstddef>
#include <numeric>
#include <thread>
#include <vector>

volatile double sink;

constexpr int num_vars = 5;
constexpr std::array<size_t, num_vars> domain_sizes = {2, 2, 2, 2, 2000};
constexpr std::array<int, num_vars> costs = {10, 10, 10, 10, 1000};

static void enumerate(const std::array<int, num_vars>& order, int depth, size_t& count, double& acc)
{
    if (depth == num_vars)
    {
        ++count;
        return;
    }
    int var = order[depth];
    double local_acc = 0.0;
    size_t domain_count = domain_sizes[var];
    for (size_t i = 0; i < domain_count; ++i)
    {
        local_acc += static_cast<double>(costs[var]) * 15.811388300841896;
    }
    acc += local_acc;
    for (size_t i = 0; i < domain_count; ++i)
    {
        enumerate(order, depth + 1, count, acc);
    }
}

size_t run(size_t loops)
{
    const std::array<int, num_vars> order = {0, 1, 2, 3, 4}; // original ordering
    size_t count = 0;
    double acc = 0.0;
    std::vector<std::thread> threads;
    for (size_t r = 0; r < loops; ++r)
    {
        threads.emplace_back(enumerate, std::cref(order), 0, std::ref(count), std::ref(acc));
    }
    for (auto& thread : threads)
        thread.join();
    sink = acc;
    return count;
}

// Explicit template instantiations
