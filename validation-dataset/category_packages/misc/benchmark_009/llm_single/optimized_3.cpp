#include <vector>
#include <limits>

struct Point {
    float x, y, z;
};

extern float pointSquaredDist(const Point& a, const Point& b);

int nn_search(const std::vector<Point>& cloud, const Point& query) {
    double smallest_squared_dist = std::numeric_limits<double>::max();
    int nearest_index = -1;
    const size_t size = cloud.size();  // Avoid multiple calls to size()
    for (size_t i = 0; i < size; ++i) {
        double dist = pointSquaredDist(cloud[i], query);  // implicit conversion
        if (dist < smallest_squared_dist) {
            smallest_squared_dist = dist;
            nearest_index = static_cast<int>(i);  // Explicit cast for clarity
        }
    }
    return nearest_index;
}
