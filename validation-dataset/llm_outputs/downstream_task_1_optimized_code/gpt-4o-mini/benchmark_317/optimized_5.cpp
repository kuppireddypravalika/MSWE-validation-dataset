#include "common.hpp"

int run_pass(const RegisterState& regs, int repeat) {
    int sum = 0;
    std::vector<Expression> simplified_values;
    simplified_values.reserve(regs.size());

    // Pre-simplify once for all register states outside of the main loop
    for (const auto& pair : regs) {
        simplified_values.push_back(pair.second.simplify());
    }

    for (int r = 0; r < repeat; ++r) {
        for (size_t i = 0; i < regs.size(); ++i) {
            auto& pair = regs[i];
            int k = pair.first;
            const Expression& v = simplified_values[i]; // Use pre-simplified value
            Expression v0(k, true);
            if (v0.equals(v))
                continue;
            if (!is_used(k))
                continue;
            sum += v.value; // Only add if other checks are passed
        }
    }
    return sum;
}