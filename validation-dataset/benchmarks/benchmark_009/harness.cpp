#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <string>

struct Point {
    float x, y, z;
};

float pointSquaredDist(const Point& a, const Point& b) {
    float dx = a.x - b.x, dy = a.y - b.y, dz = a.z - b.z;
    return dx * dx + dy * dy + dz * dz;
}

int nn_search(const std::vector<Point>& cloud, const Point& query);

std::vector<Point> generate_points(size_t count) {
    std::vector<Point> points(count);
    std::mt19937 gen(42);
    std::uniform_real_distribution<float> dist(-100.0f, 100.0f);
    for (auto& p : points) {
        p = {dist(gen), dist(gen), dist(gen)};
    }
    return points;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    size_t num_points = 500000;
    int repeat = 3000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--size=", 0) == 0) num_points = std::stoul(arg.substr(7));
        else if (arg.rfind("--repeat=", 0) == 0) repeat = std::stoi(arg.substr(9));
    }

    auto cloud = generate_points(num_points);
    Point query = {0.0f, 0.0f, 0.0f};

    if (mode == "correct") {
        int idx = nn_search(cloud, query);
        std::cout << idx << std::endl;
    } else {
        using clock = std::chrono::high_resolution_clock;

        auto t1 = clock::now();
        for (int i = 0; i < repeat; ++i)
            nn_search(cloud, query);
        auto t2 = clock::now();

        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }

    return 0;
}
