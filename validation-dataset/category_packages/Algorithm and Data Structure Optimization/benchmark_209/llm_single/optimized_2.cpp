#include <vector>
#include <memory>

enum Kind { CONST, GREATER, AND };

struct Expr {
    Kind kind;
    union {
        bool val;
        struct {
            std::size_t idx;
            double thr;
        } greater;
    } data;
    std::shared_ptr<Expr> left;
    std::shared_ptr<Expr> right;

    Expr(bool v) : kind(CONST) { data.val = v; }
    Expr(std::size_t i, double t) : kind(GREATER) { data.greater.idx = i; data.greater.thr = t; }
    Expr(std::shared_ptr<Expr> a, std::shared_ptr<Expr> b)
        : kind(AND), left(std::move(a)), right(std::move(b)) {}

    bool eval(const std::vector<double>& vars) const {
        switch (kind) {
            case CONST:   return data.val;
            case GREATER: return vars[data.greater.idx] > data.greater.thr;
            case AND:     return left->eval(vars) && right->eval(vars);
        }
        return false;
    }
};

static std::shared_ptr<Expr> make_true() { return std::make_shared<Expr>(true); }
static std::shared_ptr<Expr> make_greater(std::size_t idx, double thr) { return std::make_shared<Expr>(idx, thr); }

std::shared_ptr<Expr> and_plain(const std::shared_ptr<Expr>& a, const std::shared_ptr<Expr>& b) {
    return std::make_shared<Expr>(a, b);
}

bool build_eval(std::size_t depth, const std::vector<double>& vars) {
    auto cond = make_true();
    const std::size_t vars_size = vars.size();
    for (std::size_t i = 0; i < depth; ++i) {
        if (i % 2 == 0) {
            // conditions are always true; can be optimized by skipping
            continue;
        } else {
            cond = and_plain(cond, make_greater(i % vars_size, 0.3));
        }
    }
    return cond->eval(vars);
}

// Explicit template instantiation statements if they were present in the original code
