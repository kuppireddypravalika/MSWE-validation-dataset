#include <cmath>
#include <array>
#include <algorithm>
#include <cstddef>

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
    for (size_t i = 0; i < domain_sizes[var]; ++i)
    {
        for (int t = 0; t < costs[var]; ++t)
        {
            acc += std::sqrt(static_cast<double>(t));
        }
        enumerate(order, depth + 1, count, acc);
    }
}

static std::array<int, num_vars> order_variables_to_optimise_enumeration()
{
    std::array<int, num_vars> vars = {0, 1, 2, 3, 4};
    std::sort(vars.begin(), vars.end(), [](int a, int b) { return costs[a] > costs[b]; });
    return vars;
}

size_t run(size_t loops)
{
    const auto order = order_variables_to_optimise_enumeration();
    size_t count = 0;
    double acc = 0.0;
    for (size_t r = 0; r < loops; ++r)
    {
        enumerate(order, 0, count, acc);
    }
    sink = acc;
    return count;
}
