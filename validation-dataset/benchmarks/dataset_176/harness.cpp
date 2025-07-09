#include <chrono>
#include <iostream>
#include <string>
#include <vector>

// function declaration
size_t assemble_regions(const std::vector<int>& indices,
                        const std::vector<int>& point_labels,
                        const std::vector<int>& segment_labels,
                        const std::vector<unsigned int>& num_pts_in_region,
                        int num_regions);

static void generate_data(int num_regions, int pts_per_region,
                          std::vector<int>& indices,
                          std::vector<int>& point_labels,
                          std::vector<int>& segment_labels,
                          std::vector<unsigned int>& num_pts_in_region)
{
    segment_labels.resize(num_regions);
    num_pts_in_region.resize(num_regions);

    int total_points = 0;
    for (int i = 0; i < num_regions; ++i) {
        segment_labels[i] = i;
        if (i % 3 == 0) {
            num_pts_in_region[i] = 0; // empty cluster
        } else {
            num_pts_in_region[i] = pts_per_region;
            total_points += pts_per_region;
        }
    }

    indices.resize(total_points);
    point_labels.resize(total_points);
    int idx = 0;
    for (int i = 0; i < num_regions; ++i) {
        for (unsigned j = 0; j < num_pts_in_region[i]; ++j) {
            indices[idx] = idx;
            point_labels[idx] = i;
            ++idx;
        }
    }
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int repeat = 2500; // tuned for >10s on reference hardware
    int regions = 3000;
    int pts = 3;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--repeat=",0)==0)
            repeat = std::stoi(arg.substr(9));
        else if (arg.rfind("--regions=",0)==0)
            regions = std::stoi(arg.substr(10));
        else if (arg.rfind("--pts=",0)==0)
            pts = std::stoi(arg.substr(6));
    }

    std::vector<int> indices;
    std::vector<int> point_labels;
    std::vector<int> segment_labels;
    std::vector<unsigned int> num_pts_in_region;
    generate_data(regions, pts, indices, point_labels, segment_labels, num_pts_in_region);

    if (mode == "correct") {
        size_t out = assemble_regions(indices, point_labels, segment_labels, num_pts_in_region, regions);
        std::cout << out << "\n";
        return 0;
    }

    using clock = std::chrono::high_resolution_clock;
    size_t dummy = 0;
    auto t1 = clock::now();
    for (int r = 0; r < repeat; ++r)
        dummy += assemble_regions(indices, point_labels, segment_labels, num_pts_in_region, regions);
    auto t2 = clock::now();
    std::cout << "Time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
              << " ms\n";
    if (dummy == 123456789) std::cerr << "";
    return 0;
}
