#include <vector>
#include <cstddef>
#include <algorithm>

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

    // Reserve space upfront to avoid frequent allocations
    for (int i = 0; i < num_regions; ++i) {
        clusters[i].indices.reserve(num_pts_in_region[i]);
    }

    for (int i_point = 0; i_point < point_number; i_point++) {
        int point_index = indices[i_point];
        int index = point_labels[point_index];
        index = segment_labels[index];
        if (index < num_regions) { // Ensure index is valid
            clusters[index].indices.push_back(point_index);
        }
    }

    // Remove empty regions (compact the vector)
    clusters.erase(std::remove_if(clusters.begin(), clusters.end(), [](const PointIndices& cluster) {
        return cluster.indices.empty();
    }), clusters.end());

    return clusters.size();
}


// Explicit template instantiations
