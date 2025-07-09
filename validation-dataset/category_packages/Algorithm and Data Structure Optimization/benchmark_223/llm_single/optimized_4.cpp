#include <string>
#include <vector>

struct SkillInfo {
    std::string name;
};

int compute_max(const std::vector<SkillInfo>& skills) {
    int tmp_max = 0;
    for (const auto& info : skills) {
        int tmp_len = info.name.length();
        if (tmp_len > tmp_max) {
            tmp_max = tmp_len;
        }
    }
    return tmp_max;
}
