// original.cpp
#include <vector>
#include <cstddef>
#include <algorithm> // for std::remove_if

struct PointIndices { std::vector<int> indices; };

size_t assemble_regions(const std::vector<int>& indices,
                        const std::vector<int>& point_labels,
                        const std::vector<int>& segment_labels,
                        const std::vector<unsigned int>& num_pts_in_region,
                        int num_regions)
{
    std::vector<PointIndices> clusters(num_regions);
    std::vector<int> counter(num_regions, 0);
    int point_number = static_cast<int>(indices.size());

    // Pre-allocate indices in clusters only for regions that will actually contain points
    for (int i_seg = 0; i_seg < num_regions; i_seg++) {
        if (num_pts_in_region[i_seg] > 0) {
            clusters[i_seg].indices.resize(num_pts_in_region[i_seg]);
        }
    }

    // Assign indices to their corresponding segments in one pass
    for (int i_point = 0; i_point < point_number; i_point++) {
        int point_index = indices[i_point];
        int index = segment_labels[point_labels[point_index]];
        if (num_pts_in_region[index] > 0) {
            clusters[index].indices[counter[index]++] = point_index;
        }
    }

    // Remove empty regions more efficiently
    auto new_end = std::remove_if(clusters.begin(), clusters.end(), [](const PointIndices& pi) {
        return pi.indices.empty();
    });
    clusters.erase(new_end, clusters.end());

    return clusters.size();
}

// Explicit template instantiations if needed (reproduced unchanged)