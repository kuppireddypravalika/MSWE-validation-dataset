#include <cmath>

struct Module {
    long long loops;
    bool canonical = false;
    bool polyOptimized = false;
};

static void pollyPass(Module& m) {
    if (m.canonical)
        m.polyOptimized = true;
}

static void compile_module(Module& m) {
    pollyPass(m); // Polly runs without canonicalization
}

double run_module(const Module& m) {
    if (m.polyOptimized) {
        // Optimized path uses closed-form sine sum
        double n = static_cast<double>(m.loops);
        return std::sin((n - 1) / 2.0) * std::sin(n / 2.0) / std::sin(0.5);
    }
    // Allocate a single variable for the result instead of volatile to reduce overhead
    double sum = 0.0;
    // Pre-calculate sine(0.5) to avoid recalculating it in every iteration
    const double sine_half = std::sin(0.5);
    // Utilize a loop unrolling technique for performance improvement
    long long remaining = m.loops;
    long long i;
    for (i = 0; i + 4 <= remaining; i += 4) {
        sum += std::sin(static_cast<double>(i));
        sum += std::sin(static_cast<double>(i + 1));
        sum += std::sin(static_cast<double>(i + 2));
        sum += std::sin(static_cast<double>(i + 3));
    }
    // Handle any remaining iterations
    for (; i < remaining; ++i) {
        sum += std::sin(static_cast<double>(i));
    }
    return sum;
}

double execute(Module m) {
    compile_module(m);
    return run_module(m);
}

// Template instantiations (if required) go here