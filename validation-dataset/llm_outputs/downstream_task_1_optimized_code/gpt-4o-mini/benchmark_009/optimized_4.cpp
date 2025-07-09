#include <vector>
#include <limits>

struct Point {
    float x, y, z;
};

extern float pointSquaredDist(const Point& a, const Point& b);

int nn_search(const std::vector<Point>& cloud, const Point& query) {
    double smallest_squared_dist = std::numeric_limits<double>::max();
    int nearest_index = -1;
    const size_t cloud_size = cloud.size();
    const Point* cloud_data = cloud.data();
    for (size_t i = 0; i < cloud_size; ++i) {
        double dist = pointSquaredDist(cloud_data[i], query);
        if (dist < smallest_squared_dist) {
            smallest_squared_dist = dist;
            nearest_index = static_cast<int>(i);
        }
    }
    return nearest_index;
}
