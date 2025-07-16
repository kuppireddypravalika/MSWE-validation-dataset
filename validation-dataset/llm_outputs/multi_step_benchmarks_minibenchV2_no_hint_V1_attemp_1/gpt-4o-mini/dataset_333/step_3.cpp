#include <vector>
#include <cmath>
#include <mutex>
#include <cstdio>
#include <omp.h>

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
    void findIntersectingCells(const BoundingBox&, std::vector<size_t>& cells) {
        static thread_local bool built = false;
        if (!built) {
            std::lock_guard<std::mutex> lock(mutex_);
            build_aabb_tree();
            built = true;
        }
        cells.push_back(1);
    }
private:
    static std::mutex mutex_;
};

std::mutex Grid::mutex_;

size_t calculate_intersections(int tris) {
    Grid grid;
    std::vector<BoundingBox> boxes(tris);
    size_t total = 0;
#pragma omp parallel num_threads(6) reduction(+:total)
    {
        std::vector<size_t> out;
        out.reserve(tris);
#pragma omp for
        for (int i = 0; i < tris; ++i) {
            out.clear();
            grid.findIntersectingCells(boxes[i], out);
            total += out.size();
        }
    }
    return total;
}

// Explicit template instantiations (if any)
