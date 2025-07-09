#include "common.hpp"

int run_pass(const RegisterState& regs, int repeat) {
    int sum = 0;
    // Pre-calculate and store simplified version of expressions for reuse
    std::vector<Expression> simplified_expressions;
    simplified_expressions.reserve(regs.size());
    for (const auto& pair : regs) {
        simplified_expressions.emplace_back(pair.second.value, true);
    }

    for (int r = 0; r < repeat; ++r) {
        for (size_t i = 0; i < regs.size(); ++i) {
            const auto& pair = regs[i];
            auto k = pair.first;
            const Expression& v = pair.second;
            const Expression& v0 = simplified_expressions[i];
            // Check if 'v' equals 'v0' only once
            if (v0.equals(v))
                continue;
            if (!is_used(k))
                continue;
            Expression simplified_v = v.simplify();
            // Check if the newly simplified 'v' equals 'v0'
            if (v0.equals(simplified_v))
                continue;
            simplified_v = simplified_v.simplify();
            sum += simplified_v.value;
        }
    }
    return sum;
}
