#include "search.hpp"
#include <chrono>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <omp.h>

ParticleVector make_particles(std::size_t n) {
    ParticleVector vec;
    vec.reserve(n);
    std::mt19937 gen(42);
    std::uniform_real_distribution<double> coord(-1.0, 1.0);
    for (std::size_t i = 0; i < n; ++i) {
        auto p = std::make_shared<SphericParticle>();
        p->coord = {coord(gen), coord(gen), coord(gen)};
        p->radius = 0.05 + 0.001 * (i % 50);
        // first quarter of the particles are much heavier to create workload imbalance
        p->workload = (i < n / 4) ? 800 : 20;
        vec.push_back(p);
    }
    return vec;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    std::size_t size = 1000;
    int repeat = 10;
    int threads = std::min(4, omp_get_max_threads());

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--size=",0)==0) size = std::stoul(arg.substr(7));
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
        else if (arg.rfind("--threads=",0)==0) threads = std::stoi(arg.substr(10));
    }

    omp_set_num_threads(threads);
    ParticleVector particles = make_particles(size);

    if (mode == "correct") {
        std::size_t sum = dem_fem_search(particles);
        std::cout << sum << "\n";
        return 0;
    }

    using clock = std::chrono::high_resolution_clock;
    std::size_t dummy = 0;
    auto t1 = clock::now();
    for (int r = 0; r < repeat; ++r) {
        dummy += dem_fem_search(particles);
    }
    auto t2 = clock::now();
    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms\n";
    if (dummy == 42) std::cerr << "";
    return 0;
}
