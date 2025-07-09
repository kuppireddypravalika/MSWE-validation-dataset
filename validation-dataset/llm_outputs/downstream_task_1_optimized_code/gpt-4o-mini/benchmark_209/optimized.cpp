#include <vector>
#include <memory>

enum Kind { CONST, GREATER, AND };

struct Expr {
    Kind kind;
    bool val;
    std::size_t idx;
    double thr;
    std::shared_ptr<Expr> left;
    std::shared_ptr<Expr> right;

    Expr(bool v) : kind(CONST), val(v), idx(0), thr(0) {}
    Expr(std::size_t i, double t) : kind(GREATER), val(false), idx(i), thr(t) {}
    Expr(std::shared_ptr<Expr> a, std::shared_ptr<Expr> b)
        : kind(AND), val(false), idx(0), thr(0), left(std::move(a)), right(std::move(b)) {}

    bool eval(const std::vector<double>& vars) const {
        switch (kind) {
            case CONST:   return val;
            case GREATER: return vars[idx] > thr;
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
    std::shared_ptr<Expr> cond = make_true();
    const std::size_t var_size = vars.size();
    for (std::size_t i = 0; i < depth; ++i) {
        if (i % 2 == 0)
            cond = and_plain(cond, make_true());
        else
            cond = and_plain(cond, make_greater(i % var_size, 0.3));
    }
    return cond->eval(vars);
}

// Template instantiations
