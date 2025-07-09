#include <vector>

struct Compiler {
    std::vector<std::vector<int>> locals;
    std::vector<std::vector<int>> upvalues;
    int scope_depth = 0;

    void begin_scope() { ++scope_depth; }
    void end_scope() { --scope_depth; }

    int compile_dummy() {
        int sum = 0;
        for (int i = 0; i < 100; ++i)
            sum += i;
        return sum;
    }

    int visit_class() {
        int result = 0;
        begin_scope();
        result += compile_dummy();
        end_scope();
        result += compile_dummy();
        return result;
    }
};

int run(int repeat) {
    Compiler c;
    int out = 0;
    for (int i = 0; i < repeat; ++i)
        out += c.visit_class();
    return out;
}
