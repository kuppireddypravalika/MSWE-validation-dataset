#include <vector>
#include <limits>

struct Point {
    float x, y, z;
};

extern float pointSquaredDist(const Point& a, const Point& b);

int nn_search(const std::vector<Point>& cloud, const Point& query) {
    int nearest_index = -1;
    float smallest_squared_dist = std::numeric_limits<float>::max();

    for (size_t i = 0; i < cloud.size(); ++i) {
        float dist = pointSquaredDist(cloud[i], query);
        if (dist < smallest_squared_dist) {
            smallest_squared_dist = dist;
            nearest_index = static_cast<int>(i);
        }
    }
    return nearest_index;
}

// Explicit template instantiation statements (if any) can be added here, preserved as required.