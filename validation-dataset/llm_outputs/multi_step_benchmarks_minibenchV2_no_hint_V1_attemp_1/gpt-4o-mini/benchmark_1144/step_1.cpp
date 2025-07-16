#include <vector>
#include <cmath>
#include <random>
#ifdef _OPENMP
#include <omp.h>
#endif

static std::vector<int> g_basic;

static void init_basic(int n) {
    std::mt19937 gen(123);
    std::bernoulli_distribution d(0.3); // 30% of views are basic (heavy)
    g_basic.resize(n);
    for(int i=0;i<n;++i) g_basic[i] = d(gen);
}

static void heavy_work(std::vector<double>& v) {
    const int kIterations = 5;
    for(double& x : v) {
        double val = x;
        for(int k=0;k<kIterations; ++k) {
            val = std::sin(val) + std::cos(val);
        }
        x = val;
    }
}

void process_views(std::vector<std::vector<double>>& views) {
    if(g_basic.empty()) init_basic(static_cast<int>(views.size()));
#ifdef _OPENMP
#pragma omp parallel for
#endif
    for(int i=0; i<(int)views.size(); ++i) {
        if(g_basic[i]) { // Check if the view is heavy
            heavy_work(views[i]);
        }
    }
}


// Explicit template instantiations
