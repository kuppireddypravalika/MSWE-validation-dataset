#include <vector>
#include <cmath>
#include <random>
#include <algorithm> // for std::transform
#ifdef _OPENMP
#include <omp.h>
#endif

static std::vector<int> g_basic;

static void init_basic(int n) {
    std::mt19937 gen(123);
    std::bernoulli_distribution d(0.3);
    g_basic.resize(n);
    for (int i = 0; i < n; ++i) g_basic[i] = d(gen);
}

static void heavy_work(std::vector<double>& v) {
    std::transform(v.begin(), v.end(), v.begin(), [](double x) {
        for (int k = 0; k < 5; ++k) {
            x = std::sin(x) + std::cos(x);
        }
        return x;
    });
}

void process_views(std::vector<std::vector<double>>& views) {
    if (g_basic.empty()) init_basic(static_cast<int>(views.size()));
#ifdef _OPENMP
#pragma omp parallel for shared(views)
#endif
    for (int i = 0; i < static_cast<int>(views.size()); ++i) {
        if (!g_basic[i]) continue; // trivial view
        heavy_work(views[i]);
    }
}


// Explicit template instantiations
#include <vector>
template void process_views(std::vector<std::vector<double>>& views);