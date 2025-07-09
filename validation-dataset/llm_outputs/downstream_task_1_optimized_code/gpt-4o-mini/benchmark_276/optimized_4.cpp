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

inline double compute_sin_sum(long long loops) {
    double n = static_cast<double>(loops);
    return std::sin((n - 1) / 2.0) * std::sin(n / 2.0) / std::sin(0.5);
}

double run_module(const Module& m) {
    if (m.polyOptimized) {
        return compute_sin_sum(m.loops);
    }
    double sum = 0.0;
    for (long long i = 0; i < m.loops; ++i)
        sum += std::sin(static_cast<double>(i));
    return sum;
}

double execute(Module m) {
    compile_module(m);
    return run_module(m);
}

// Explicit template instantiation statements (if there were any) should go here.