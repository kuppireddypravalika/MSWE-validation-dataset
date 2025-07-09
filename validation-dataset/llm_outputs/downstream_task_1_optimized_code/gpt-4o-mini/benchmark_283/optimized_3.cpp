// optimized.cpp
#include <vector>
#include <memory>
#include <numeric> // For std::accumulate

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

static void original_convert(const std::vector<Point>& cloud,
                             const std::vector<int>& indices,
                             std::unique_ptr<float[]>& cloud_out,
                             std::vector<int>& index_mapping,
                             const PointRepresentation& repr)
{
    if (cloud.empty()) {
        cloud_out.reset();
        return;
    }
    int dim = repr.getNumberOfDimensions();
    int original_no_of_points = static_cast<int>(indices.size());
    cloud_out.reset(new float[static_cast<size_t>(original_no_of_points) * dim]);
    float* cloud_ptr = cloud_out.get();
    index_mapping.reserve(original_no_of_points);

    for (int index : indices) // Using range-based for loop
    {
        const Point& p = cloud[index]; // Local reference to avoid repeated indexing
        if (!repr.isValid(p))
            continue;

        index_mapping.push_back(index);
        repr.vectorize(p, cloud_ptr);
        cloud_ptr += dim;
    }
}

unsigned long convert(const std::vector<Point>& cloud,
                      const std::vector<int>& indices) {
    std::unique_ptr<float[]> cloud_out;
    std::vector<int> index_mapping;
    PointRepresentation repr;
    original_convert(cloud, indices, cloud_out, index_mapping, repr);
    return std::accumulate(index_mapping.begin(), index_mapping.end(), 0UL); // Using std::accumulate for cleaner sum
}

// Explicit template instantiation statements would go here if needed.