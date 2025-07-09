#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

// Struct and function declarations shared with implementations
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


bool build_eval(std::size_t depth, const std::vector<double>& vars);

std::vector<double> create_vars(std::size_t n) {
    std::vector<double> v(n);
    for (std::size_t i = 0; i < n; ++i) v[i] = static_cast<double>(i % 100) / 100.0;
    return v;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    std::size_t vars = 100;
    std::size_t depth = 5000;
    int repeat = 20000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--vars=", 0) == 0) vars = std::stoul(arg.substr(7));
        else if (arg.rfind("--depth=", 0) == 0) depth = std::stoul(arg.substr(8));
        else if (arg.rfind("--repeat=", 0) == 0) repeat = std::stoi(arg.substr(9));
    }

    auto data = create_vars(vars);

    if (mode == "correct") {
        bool res = build_eval(depth, data);
        std::cout << res << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int r = 0; r < repeat; ++r)
            build_eval(depth, data);
        auto t2 = clock::now();

        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
    return 0;
}
