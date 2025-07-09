#include <vector>
#include <cmath>
#include <mutex>
#include <cstdio>

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
    void preBuild() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (!built_) {
            build_aabb_tree();
            built_ = true;
        }
    }
    void findIntersectingCells(const BoundingBox&, std::vector<size_t>* cells) {
        cells->push_back(1);
    }
private:
    bool built_ = false;
    std::mutex mutex_;
};

size_t calculate_intersections(int tris) {
    Grid grid;
    grid.preBuild();
    std::vector<BoundingBox> boxes(tris);
    size_t total = 0;
#pragma omp parallel num_threads(6) reduction(+:total)
    {
        std::vector<size_t> out;
#pragma omp for
        for (int i = 0; i < tris; ++i) {
            grid.findIntersectingCells(boxes[i], &out);
            total += out.size();
            out.clear();
        }
    }
    return total;
}
