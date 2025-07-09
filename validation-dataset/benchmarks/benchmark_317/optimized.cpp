#include "common.hpp"

int run_pass(const RegisterState& regs, int repeat) {
    int sum = 0;
    for (int r = 0; r < repeat; ++r) {
        for (auto& pair : regs) {
            auto k = pair.first;
            auto v = pair.second.simplify();
            Expression v0(k, true);
            if (v0.equals(v))
                continue;
            if (!is_used(k))
                continue;
            v = v.simplify();
            sum += v.value;
        }
    }
    return sum;
}
