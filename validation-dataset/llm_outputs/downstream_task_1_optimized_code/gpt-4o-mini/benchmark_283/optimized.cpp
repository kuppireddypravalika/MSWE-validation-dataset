#include <vector>
#include <memory>

struct Point {
    float x, y, z;
    bool valid;
};

struct PointRepresentation {
    int getNumberOfDimensions() const { return 3; }
    bool isValid(const Point& p) const { return p.valid; }
    void vectorize(const Point& p, float* out) const {
        out[0] = p.x;
        out[1] = p.y;
        out[2] = p.z;
    }
};

unsigned long convert(const std::vector<Point>& cloud,
                      const std::vector<int>& indices) {
    if (cloud.empty()) {
        return 0;
    }
    int dim = 3;
    int original_no_of_points = static_cast<int>(indices.size());
    std::unique_ptr<float[]> cloud_out(new float[static_cast<size_t>(original_no_of_points) * dim]);
    float* cloud_ptr = cloud_out.get();
    std::vector<int> index_mapping;
    index_mapping.reserve(original_no_of_points);
    unsigned long sum = 0;

    for (int idx : indices) {
        const Point& p = cloud[idx];
        if (!p.valid) continue;
        index_mapping.push_back(idx);
        cloud_ptr[0] = p.x;
        cloud_ptr[1] = p.y;
        cloud_ptr[2] = p.z;
        cloud_ptr += dim;
        sum += static_cast<unsigned long>(idx);
    }
    return sum;
}
