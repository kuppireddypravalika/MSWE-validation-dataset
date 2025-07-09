#include <iostream>
#include <vector>
#include <string>
#include <chrono>

struct SkillInfo {
    std::string name;
};

int compute_max(const std::vector<SkillInfo>& skills);

std::vector<SkillInfo> generate_skills(size_t count, size_t len) {
    std::vector<SkillInfo> skills;
    skills.reserve(count);
    std::string base(len, 'a');
    for (size_t i = 0; i < count; ++i) {
        skills.push_back({base});
    }
    return skills;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    size_t count = 40000;  // number of skills
    size_t len = 100;      // length of each skill name
    int repeat = 8000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--count=", 0) == 0)
            count = std::stoul(arg.substr(8));
        else if (arg.rfind("--len=", 0) == 0)
            len = std::stoul(arg.substr(6));
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stoi(arg.substr(9));
    }

    auto skills = generate_skills(count, len);

    if (mode == "correct") {
        int result = compute_max(skills);
        std::cout << result << std::endl;
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int i = 0; i < repeat; ++i) {
            compute_max(skills);
        }
        auto t2 = clock::now();

        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
    return 0;
}
