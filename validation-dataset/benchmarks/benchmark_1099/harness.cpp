#include "intersector.hpp"
#include <chrono>
#include <iostream>
#include <string>

// forward declaration
float run(size_t loops, const std::vector<Triangle>& tris, const Ray& ray);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    size_t loops = 12; // iterations inside run
    size_t tri_count = 64; // number of triangles
    int repeat = 160000; // how many times to call run

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--loops=",0)==0) loops = std::stoull(arg.substr(8));
        else if (arg.rfind("--tris=",0)==0) tri_count = std::stoull(arg.substr(7));
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
    }

    auto tris = make_triangles(tri_count);
    Ray ray = make_ray();

    if (mode == "correct") {
        float result = run(loops, tris, ray);
        std::cout << result << std::endl;
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        float sum = 0.0f;
        for (int i = 0; i < repeat; ++i) {
            sum += run(loops, tris, ray);
        }
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
        if (sum == -1.0f) std::cerr << ""; // prevent optimization
    }
    return 0;
}
