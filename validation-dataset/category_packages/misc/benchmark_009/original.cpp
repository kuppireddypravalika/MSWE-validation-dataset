#include <vector>
#include <limits>

struct Point {
    float x, y, z;
};

extern float pointSquaredDist(const Point& a, const Point& b);

int nn_search(const std::vector<Point>& cloud, const Point& query) {
    double smallest_squared_dist = std::numeric_limits<double>::max();
    int nearest_index = -1;
    for (int i = 0; i < cloud.size(); ++i) {
        double dist = pointSquaredDist(cloud[i], query);  // implicit conversion
        if (dist < smallest_squared_dist) {
            smallest_squared_dist = dist;
            nearest_index = i;
        }
    }
    return nearest_index;
}
