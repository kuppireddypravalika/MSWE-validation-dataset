#pragma once
#include <cmath>
#include <vector>

struct Expression {
    int value;
    bool simplified;
    Expression(int v = 0, bool s = false) : value(v), simplified(s) {}
    static void heavy() {
        volatile double x = 0;
        for (int i = 0; i < 1000; ++i)
            x += std::sqrt(i + x);
    }
    Expression simplify() const {
        heavy();
        return Expression(value, true);
    }
    bool equals(const Expression& other) const {
        if (!simplified)
            heavy();
        if (!other.simplified)
            heavy();
        return value == other.value;
    }
};

using RegisterState = std::vector<std::pair<int, Expression>>;

inline bool is_used(int) {
    Expression::heavy();
    return true;
}
