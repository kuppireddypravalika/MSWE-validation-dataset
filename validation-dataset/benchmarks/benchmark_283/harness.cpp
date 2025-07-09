#include <vector>
#include <memory>
#include <random>
#include <chrono>
#include <iostream>
#include <string>

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
                      const std::vector<int>& indices);

std::vector<Point> generate_cloud(size_t n) {
    std::vector<Point> pts(n);
    std::mt19937 gen(42);
    std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
    std::bernoulli_distribution valid_dist(0.9);
    for (auto& p : pts) {
        p.x = dist(gen);
        p.y = dist(gen);
        p.z = dist(gen);
        p.valid = valid_dist(gen);
    }
    return pts;
}

std::vector<int> generate_indices(size_t n) {
    std::vector<int> idx(n);
    for (size_t i = 0; i < n; ++i)
        idx[i] = static_cast<int>(i);
    return idx;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    size_t size = 500000;
    int repeat = 5000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--size=", 0) == 0) size = std::stoul(arg.substr(7));
        else if (arg.rfind("--repeat=", 0) == 0) repeat = std::stoi(arg.substr(9));
    }

    auto cloud = generate_cloud(size);
    auto indices = generate_indices(size);
    PointRepresentation repr;
    std::vector<int> mapping;
    std::unique_ptr<float[]> array;

    if (mode == "correct") {
        unsigned long sum = convert(cloud, indices);
        std::cout << sum << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int r = 0; r < repeat; ++r)
            convert(cloud, indices);
        auto t2 = clock::now();

        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
    return 0;
}
