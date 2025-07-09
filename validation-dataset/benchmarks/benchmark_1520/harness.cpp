#include "distance_table.hpp"
#include <chrono>
#include <iostream>
#include <string>
#include <random>
#include <omp.h>

void evaluate(SoaDistanceTableBA& table, ParticleSet& P);

ParticleSet create_sources(int n) {
    ParticleSet p; p.RSoA.resize(n);
    std::mt19937 gen(42);
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    for (auto &v : p.RSoA)
        for (double &x : v) x = dist(gen);
    return p;
}

ParticleSet create_targets(int n) {
    ParticleSet p; p.R.resize(n);
    std::mt19937 gen(1337);
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    for (auto &v : p.R)
        for (double &x : v) x = dist(gen);
    return p;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    std::string impl = "original"; // parsed for compatibility
    int sources = 2048;
    int targets = 2048;
    int repeat = 800;
    int threads = std::min(4, omp_get_max_threads());


    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg == "--impl=optimized") impl = "optimized";
        else if (arg == "--impl=original") impl = "original";
        else if (arg.rfind("--sources=",0)==0) sources = std::stoi(arg.substr(10));
        else if (arg.rfind("--targets=",0)==0) targets = std::stoi(arg.substr(10));
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
        else if (arg.rfind("--threads=",0)==0) threads = std::stoi(arg.substr(10));
    }

    omp_set_num_threads(threads);


    ParticleSet src = create_sources(sources);
    ParticleSet tgt = create_targets(targets);
    SoaDistanceTableBA table(src, tgt);

    if (mode == "correct") {
        evaluate(table, tgt);
        double sum = 0.0;
        for (const auto& row : table.Distances)
            for (double d : row) sum += d;
        std::cout << sum << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int r = 0; r < repeat; ++r) evaluate(table, tgt);
        auto t2 = clock::now();
        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms\n";
    }
    return 0;
}
