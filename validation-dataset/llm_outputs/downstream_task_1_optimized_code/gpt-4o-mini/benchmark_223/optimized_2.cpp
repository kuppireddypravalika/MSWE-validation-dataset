#include <string>
#include <vector>
#include <algorithm>

struct SkillInfo {
    std::string name;
};

int compute_max(const std::vector<SkillInfo>& skills) {
    int max_length = 0;
    for (const auto& info : skills) {
        max_length = std::max(max_length, static_cast<int>(info.name.length()));
    }
    return max_length;
}
