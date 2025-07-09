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

// Simulate copy propagation using on-demand use-def queries
uint64_t copy_propagation(UseDefInfo& info) {
    uint64_t sum = 0;
    for (int d = 0; d < info.num_defs; ++d) {
        std::vector<int>& uses = info.uses_of_def[d];
        if (!info.built) {
            info.getUsesFromDef(uses, d);
        }
        for (int u : uses)
            sum += static_cast<uint64_t>(u);
    }
    return sum;
}

// Explicit template instantiation statements (if any) would go here