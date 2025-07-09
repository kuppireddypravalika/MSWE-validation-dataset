#include <cmath>
#include <array>
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
    const size_t domain_size = domain_sizes[var];
    const int cost = costs[var];
    const double sqrt_0_to_9[10] = {0, 1, 1.41421356237, 1.73205080757, 2, 2.2360679775, 2.44948974278, 2.64575131106, 2.82842712475, 3};
    for (size_t i = 0; i < domain_size; ++i)
    {
        double local_acc = 0.0;
        for (int t = 0; t < cost && t < 10; ++t) // bounding t for precomputation
        {
            local_acc += sqrt_0_to_9[t];
        }
        acc += local_acc;
        if (cost > 10) // handle costs > 10
        {
            acc += (cost - 10) * (sqrt_0_to_9[9] + std::sqrt(10)); // approximate for t > 9
        }
        enumerate(order, depth + 1, count, acc);
    }
}

size_t run(size_t loops)
{
    const std::array<int, num_vars> order = {0, 1, 2, 3, 4}; // original ordering
    size_t count = 0;
    double acc = 0.0;
    for (size_t r = 0; r < loops; ++r)
    {
        enumerate(order, 0, count, acc);
    }
    sink = acc;
    return count;
}

// Template instantiation statements
