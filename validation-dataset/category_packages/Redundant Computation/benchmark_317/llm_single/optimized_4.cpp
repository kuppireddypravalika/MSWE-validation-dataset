#include "common.hpp"

int run_pass(const RegisterState& regs, int repeat) {
    int sum = 0;
    std::vector<Expression> simplified_expressions;
    simplified_expressions.reserve(regs.size());

    // Pre-simplify all expressions in regs
    for (const auto& pair : regs) {
        simplified_expressions.emplace_back(pair.second.simplify());
    }

    for (int r = 0; r < repeat; ++r) {
        for (size_t i = 0; i < regs.size(); ++i) {
            auto k = regs[i].first;
            const auto& v = simplified_expressions[i];
            Expression v0(k, true);
            if (v0.equals(v))
                continue;
            if (!is_used(k))
                continue;
            // No need to simplify again, use the pre-computed value
            sum += v.value;
        }
    }
    return sum;
}
