#include <string>
#include <vector>

struct SkillInfo {
    std::string name;
};

int compute_max(const std::vector<SkillInfo>& skills) {
    int tmp_max = 0;
    int tmp_len;
    for (auto it = skills.begin(); it != skills.end(); ++it) {
        SkillInfo info = *it;
        tmp_len = info.name.size();
        if (tmp_len > tmp_max) tmp_max = tmp_len;
    }
    return tmp_max;
}
