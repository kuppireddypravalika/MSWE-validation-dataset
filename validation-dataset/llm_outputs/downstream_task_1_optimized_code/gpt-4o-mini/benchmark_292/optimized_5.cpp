#include <vector>

struct Compiler {
    std::vector<int> locals;
    std::vector<int> upvalues;
    int scope_depth = 0;

    void begin_scope() { ++scope_depth; }
    void end_scope() { --scope_depth; }

    int compile_dummy() {
        return (99 * 100) / 2; // Sum of first 99 numbers
    }

    int visit_class() {
        int result = 0;
        begin_scope();
        locals.resize(1000);
        upvalues.resize(1000);
        for (int i = 0; i < 1000; ++i) {
            locals[i] = i;
            upvalues[i] = i;
        }
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

// Explicit template instantiation statements go here (unchanged)