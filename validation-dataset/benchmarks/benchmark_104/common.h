#ifndef COMMON_H
#define COMMON_H
#include <memory>
#include <vector>
#include <string>

// Simplified stand-ins for GDAL dimension types
enum DimType { GDAL_DIM_TYPE_HORIZONTAL_Y, GDAL_DIM_TYPE_HORIZONTAL_X, OTHER };
struct Dimension { DimType t; };

// Minimal representation of GDALMDArray
class MDArray {
    int count;
    std::vector<Dimension> dims;
public:
    MDArray(int c = 0, DimType t0 = OTHER, DimType t1 = OTHER) : count(c) {
        if (c >= 1) dims.push_back({t0});
        if (c >= 2) dims.push_back({t1});
    }
    int GetDimensionCount() const { return count; }
    const std::vector<Dimension>& GetDimensions() const { return dims; }
};

// Minimal group with named arrays
class Group {
public:
    std::vector<std::string> names;
    std::vector<std::shared_ptr<MDArray>> arrays;

    std::vector<std::string> GetGroupNames() const { return {}; }
    const std::vector<std::string>& GetMDArrayNames() const { return names; }
    std::shared_ptr<MDArray> OpenMDArray(const std::string& name) const {
        for (size_t i = 0; i < names.size(); ++i) {
            if (names[i] == name) return arrays[i];
        }
        return nullptr;
    }
};

struct Dataset {
    Group group;
    Group* GetRootGroup() const { return const_cast<Group*>(&group); }
};

// Construct a dataset with a single 2D array candidate
inline Dataset createDataset() {
    Dataset ds;
    ds.group.names = {"dummy", "candidate"};
    ds.group.arrays = {
        std::make_shared<MDArray>(1),
        std::make_shared<MDArray>(2, GDAL_DIM_TYPE_HORIZONTAL_Y, GDAL_DIM_TYPE_HORIZONTAL_X)
    };
    return ds;
}

#endif // COMMON_H
