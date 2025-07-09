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
    double sqrt_vals[1000]; // Precompute square roots to avoid recomputation
    for (int t = 0; t < costs[var]; ++t)
    {
        sqrt_vals[t] = std::sqrt(static_cast<double>(t));  // Store in an array
    }
    for (size_t i = 0; i < domain_sizes[var]; ++i)
    {
        acc += sqrt_vals[i < costs[var] ? i : costs[var] - 1]; // Use precomputed values
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

// Explicit template instantiation statements
