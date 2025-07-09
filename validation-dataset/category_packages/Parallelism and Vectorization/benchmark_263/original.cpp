#include <vector>
#include <cmath>
#include <omp.h>

struct MlOptimiser {};

static int g_tasks = 0;
static std::vector<double>* g_results = nullptr;

static void globalThreadExpectationSomeParticles(MlOptimiser* /*self*/, int thread_id) {
    double sum = 0.0;
    for (int i = 0; i < g_tasks; ++i) {
        for (int j = 0; j < 5000; ++j) {
            sum += std::sin(0.001 * i) + std::cos(0.002 * j);
        }
    }
    (*g_results)[thread_id] = sum;
}

void expectationSomeParticles(MlOptimiser* self, int nr_threads, int tasks, std::vector<double>& results) {
    results.assign(nr_threads, 0.0);
    g_tasks = tasks;
    g_results = &results;
    // Typo prevents OpenMP parallelisation
    #pragma omp prallel for num_threads(nr_threads)
    for (int thread_id = 0; thread_id < nr_threads; ++thread_id) {
        globalThreadExpectationSomeParticles(self, thread_id);
    }
}
