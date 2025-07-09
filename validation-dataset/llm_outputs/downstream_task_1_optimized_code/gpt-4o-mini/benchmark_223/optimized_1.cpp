#include <string>
#include <vector>
#include <algorithm>

struct SkillInfo {
    std::string name;
};

int compute_max(const std::vector<SkillInfo>& skills) {
    int tmp_max = 0;
    for (const auto& info : skills) {
        // Use string length directly instead of using c_str and strlen
        tmp_max = std::max(tmp_max, static_cast<int>(info.name.length()));
    }
    return tmp_max;
}

// Explicit template instantiations if any
