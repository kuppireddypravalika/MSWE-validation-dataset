#include <vector>
#include <cmath>
#include <omp.h>

struct MlOptimiser {};

void expectationSomeParticles(MlOptimiser* self, int nr_threads, int tasks, std::vector<double>& results) {
    results.assign(nr_threads, 0.0);
    
    #pragma omp parallel for num_threads(nr_threads) schedule(static)
    for (int thread_id = 0; thread_id < nr_threads; ++thread_id) {
        double sum = 0.0;
        for (int i = 0; i < tasks; ++i) {
            for (int j = 0; j < 5000; ++j) {
                sum += std::sin(0.001 * i) + std::cos(0.002 * j);
            }
        }
        results[thread_id] = sum;
    }
}