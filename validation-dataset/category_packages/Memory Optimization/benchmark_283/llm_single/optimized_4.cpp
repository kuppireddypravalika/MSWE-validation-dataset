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
    if (cloud.empty()) return 0;

    const int dim = 3;
    const int original_no_of_points = static_cast<int>(indices.size());
    std::vector<float> cloud_out(original_no_of_points * dim);
    std::vector<int> index_mapping;
    index_mapping.reserve(original_no_of_points);

    float* cloud_ptr = cloud_out.data();
    const PointRepresentation repr;

    for (const int index : indices) {
        const Point& p = cloud[index];
        if (!repr.isValid(p)) continue;

        index_mapping.push_back(index);
        repr.vectorize(p, cloud_ptr);
        cloud_ptr += dim;
    }

    unsigned long sum = 0;
    for (const int idx : index_mapping) sum += static_cast<unsigned long>(idx);
    return sum;
}
