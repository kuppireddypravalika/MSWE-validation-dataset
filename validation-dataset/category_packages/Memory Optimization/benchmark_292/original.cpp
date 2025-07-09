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
        locals.emplace_back(1000, 0);
        upvalues.emplace_back(1000, 0);
        for (int i = 0; i < 1000; ++i) {
            locals.back()[i] = i;
            upvalues.back()[i] = i;
        }
        result += compile_dummy();
        end_scope();
        locals.pop_back();
        upvalues.pop_back();
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
