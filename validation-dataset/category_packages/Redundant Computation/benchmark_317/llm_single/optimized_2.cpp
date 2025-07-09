#include "common.hpp"

int run_pass(const RegisterState& regs, int repeat) {
    int sum = 0;
    // Pre-compute simplified expressions to avoid redundant calculations
    std::vector<Expression> simplifiedExpressions;
    simplifiedExpressions.reserve(regs.size());
    for (const auto& pair : regs) {
        simplifiedExpressions.emplace_back(pair.first, true);
    }

    for (int r = 0; r < repeat; ++r) {
        for (size_t i = 0; i < regs.size(); ++i) {
            const auto& pair = regs[i];
            auto k = pair.first;
            const auto& originalExpr = pair.second;
            auto& v0 = simplifiedExpressions[i];

            if (v0.equals(originalExpr))
                continue;
            if (!is_used(k))
                continue;

            Expression v = originalExpr.simplify(); // Simplify once
            if (v0.equals(v))
                continue;
            v = v.simplify(); // Simplify again

            sum += v.value;
        }
    }
    return sum;
}
