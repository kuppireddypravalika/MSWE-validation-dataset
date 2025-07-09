#include "common.hpp"

int run_pass(const RegisterState& regs, int repeat) {
    int sum = 0;
    for (int r = 0; r < repeat; ++r) {
        for (const auto& pair : regs) {
            const int k = pair.first;
            const Expression& v = pair.second;
            Expression v0(k, true);
            if (v0.equals(v))
                continue;
            if (!is_used(k))
                continue;
            Expression simplified_v = v.simplify();
            if (v0.equals(simplified_v))
                continue;
            // Second simplification only if necessary
            simplified_v = simplified_v.simplify();
            sum += simplified_v.value;
        }
    }
    return sum;
}
