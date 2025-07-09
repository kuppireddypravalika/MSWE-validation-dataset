#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <cstdint>

struct UseDefInfo {
    int num_defs;
    int num_uses;
    std::vector<int> use_to_def;
    bool built;
    std::vector<std::vector<int>> uses_of_def;

    UseDefInfo(int defs, int uses)
        : num_defs(defs), num_uses(uses), use_to_def(uses), built(false) {
        std::mt19937 gen(42);
        std::uniform_int_distribution<int> dist(0, defs - 1);
        for (int i = 0; i < uses; ++i)
            use_to_def[i] = dist(gen);
    }

    void buildDefUseInfo() {
        uses_of_def.assign(num_defs, {});
        for (int i = 0; i < num_uses; ++i)
            uses_of_def[use_to_def[i]].push_back(i);
        built = true;
    }

    void getUsesFromDef(std::vector<int>& out, int def) {
        if (built) {
            out = uses_of_def[def];
        } else {
            out.clear();
            for (int i = 0; i < num_uses; ++i)
                if (use_to_def[i] == def)
                    out.push_back(i);
        }
    }
};

uint64_t copy_propagation(UseDefInfo& info);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int defs = 1000;
    int uses = 10000;
    int repeat = 1200;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--defs=", 0) == 0)
            defs = std::stoi(arg.substr(7));
        else if (arg.rfind("--uses=", 0) == 0)
            uses = std::stoi(arg.substr(7));
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stoi(arg.substr(9));
    }

    UseDefInfo info(defs, uses);

    if (mode == "correct") {
        uint64_t result = copy_propagation(info);
        std::cout << result << '\n';
        return 0;
    }

    using clock = std::chrono::high_resolution_clock;

    auto t1 = clock::now();
    for (int i = 0; i < repeat; ++i) {
        info.built = false;
        copy_propagation(info);
    }
    auto t2 = clock::now();

    std::cout << "Time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
              << " ms\n";

    return 0;
}
