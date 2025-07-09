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

    // Precompute sqrt values to avoid redundant calculations
    std::array<double, 10> sqrt_values;
    for (int t = 0; t < costs[var]; ++t)
    {
        sqrt_values[t] = std::sqrt(static_cast<double>(t));
    }

    // Use single loop instead of nested loop to enhance performance
    const size_t domain_size = domain_sizes[var];
    for (size_t i = 0; i < domain_size; ++i)
    {
        // Accumulate sqrt values based on costs
        acc += sqrt_values[costs[var] - 1]; // Only the last one is used in the original
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

// Explicit template instantiation statements (unchanged)
