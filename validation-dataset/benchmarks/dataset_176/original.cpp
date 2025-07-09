#include <vector>
#include <cstddef>

struct PointIndices { std::vector<int> indices; };

size_t assemble_regions(const std::vector<int>& indices,
                        const std::vector<int>& point_labels,
                        const std::vector<int>& segment_labels,
                        const std::vector<unsigned int>& num_pts_in_region,
                        int num_regions)
{
    std::vector<PointIndices> clusters;
    clusters.clear();
    PointIndices segment;
    clusters.resize(num_regions, segment);
    for (int i_seg = 0; i_seg < num_regions; i_seg++) {
        clusters[i_seg].indices.resize(num_pts_in_region[i_seg]);
    }

    std::vector<int> counter(num_regions, 0);
    int point_number = static_cast<int>(indices.size());
    for (int i_point = 0; i_point < point_number; i_point++) {
        int point_index = indices[i_point];
        int index = point_labels[point_index];
        index = segment_labels[index];
        clusters[index].indices[counter[index]] = point_index;
        counter[index] += 1;
    }

    // now we need to erase empty regions
    auto i_region = clusters.begin();
    while (i_region != clusters.end()) {
        if (i_region->indices.empty())
            i_region = clusters.erase(i_region);
        else
            ++i_region;
    }

    return clusters.size();
}
