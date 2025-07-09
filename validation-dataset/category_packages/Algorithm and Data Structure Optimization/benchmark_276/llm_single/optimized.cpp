#include <cmath>

struct Module {
    long long loops;
    bool canonical = false;
    bool polyOptimized = false;
};

static void compile_module(Module& m) {
    if (m.canonical)
        m.polyOptimized = true;
}

volatile double run_module(const Module& m) {
    if (m.polyOptimized) {
        // Optimized path uses closed-form sine sum
        double n = static_cast<double>(m.loops);
        double halfN = n / 2.0;
        return std::sin((n - 1) / 2.0) * std::sin(halfN) / std::sin(0.5);
    }
    // Use a more efficient loop for sum calculation
    volatile double sum = 0.0;
    double tempSin = 0.0;
    for (long long i = 0; i < m.loops; ++i) {
        tempSin = std::sin(static_cast<double>(i));
        sum += tempSin;
    }
    return sum;
}

double execute(Module m) {
    compile_module(m);
    return run_module(m);
}

// Explicit template instantiations
