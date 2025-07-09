#include <vector>
#include <memory>
#include <numeric>

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
    if (cloud.empty() || indices.empty()) return 0;
    
    int dim = 3;
    int original_no_of_points = static_cast<int>(indices.size());
    std::unique_ptr<float[]> cloud_out(new float[static_cast<size_t>(original_no_of_points) * dim]);
    float* cloud_ptr = cloud_out.get();
    std::vector<int> index_mapping;
    index_mapping.reserve(original_no_of_points);
    
    for (int idx : indices) {
        if (cloud[idx].valid) {
            index_mapping.push_back(idx);
            PointRepresentation repr;
            repr.vectorize(cloud[idx], cloud_ptr);
            cloud_ptr += dim;
        }
    }
    
    return std::accumulate(index_mapping.begin(), index_mapping.end(), 0ULL);
}