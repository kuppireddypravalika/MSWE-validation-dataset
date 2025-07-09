#include "common.h"
#include <chrono>
#include <iostream>
#include <string>

bool intersectRay(const Ray& r, const AABox& bounds, float maxDist);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    // Workload tuned so the original implementation runs for over five seconds
    unsigned long long repeat = 1000000000ULL;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stoull(arg.substr(9));
    }

    Vec3 org{{0.f, 0.f, 0.f}};
    Vec3 dir{{1.f, -1.f, 0.5f}};
    Ray ray(org, dir);
    AABox box{{{-10.f, -10.f, -10.f}}, {{10.f, 10.f, 10.f}}};
    float maxDist = 1000.f;
    volatile bool sink = false;

    if (mode == "correct") {
        sink = intersectRay(ray, box, maxDist);
        std::cout << static_cast<int>(sink) << "\n";
    } else {
        using clock = std::chrono::high_resolution_clock;
    auto t1 = clock::now();
    for (unsigned long long i = 0; i < repeat; ++i)
        sink ^= intersectRay(ray, box, maxDist);
    auto t2 = clock::now();

    std::cout << "Time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
              << " ms\n";
    }
    return sink ? 0 : 0;
}
