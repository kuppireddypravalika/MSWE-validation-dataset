#include <string>
#include <vector>
#include <algorithm>

struct SkillInfo {
    std::string name;
};

int compute_max(const std::vector<SkillInfo>& skills) {
    int tmp_max = 0;
    for (const auto& info : skills) {
        int tmp_len = static_cast<int>(info.name.length());
        tmp_max = std::max(tmp_max, tmp_len);
    }
    return tmp_max;
}
