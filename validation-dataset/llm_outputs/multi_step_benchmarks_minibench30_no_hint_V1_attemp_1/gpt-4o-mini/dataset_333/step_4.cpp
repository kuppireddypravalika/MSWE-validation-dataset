// original.cpp
#include <vector>
#include <cmath>
#include <cstdio>
#include <omp.h>
#include <mutex>

struct BoundingBox { double dummy[2]; };

static void build_aabb_tree() {
    volatile double sum = 0.0;
    for (size_t i = 0; i < 180000000; ++i) {
        sum += std::sin(i * 0.00001);
    }
    if (sum < 0) std::printf("%f", sum);
}

class Grid {
public:
    void findIntersectingCells(const BoundingBox& box, std::vector<size_t>* cells) {
        static std::once_flag init_flag;
        std::call_once(init_flag, &build_aabb_tree);

        cells->clear(); // Prepare cells vector for results
        cells->push_back(1); // Mock intersection found
    }
};

size_t calculate_intersections(int tris) {
    Grid grid;
    std::vector<BoundingBox> boxes(tris);
    size_t total = 0;
    
    // Parallelize the calculation of intersections
    #pragma omp parallel reduction(+:total)
    {
        std::vector<size_t> out;
        #pragma omp for nowait
        for (int i = 0; i < tris; ++i) {
            grid.findIntersectingCells(boxes[i], &out);
            total += out.size();
            out.clear(); // Ensure clean output for subsequent iterations
        }
    }
    return total;
}

// Explicit template instantiations remain unchanged