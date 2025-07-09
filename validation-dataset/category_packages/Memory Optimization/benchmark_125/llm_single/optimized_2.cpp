#include <functional>
#include <string>

namespace optimized {

struct Label {
    int* block;
    Label() : block(new int[64]()) {} // Initialize memory to zero
    explicit Label(int* b) : block(b) {}
    Label(Label&& o) noexcept : block(o.block) { o.block = nullptr; }
    ~Label() { delete[] block; }
    Label(const Label&) = delete;
    Label& operator=(const Label&) = delete;
};

inline void insn_label(Label* l) {
    l->block[0]++;
}

inline void insn_branch(Label* l) { insn_label(l); }

void insn_if(bool condition, const std::function<void()>& then_fn, const std::function<void()>& else_fn) {
    Label label_then;
    Label label_else;
    Label label_end;

    if (condition) {
        insn_label(&label_then);
        if (then_fn) then_fn();
        insn_branch(&label_end);
    } else if (else_fn) {
        insn_label(&label_else);
        else_fn();
        insn_branch(&label_end);
    } else {
        insn_branch(&label_end);
    }
    insn_label(&label_end);
}

int run(long long iterations) {
    int result = 0;
    auto then_fn = [&]() { result++; };
    for (long long i = 0; i < iterations; ++i) {
        insn_if(true, then_fn, nullptr);
    }
    return result;
}

} // namespace optimized

int run(long long iterations) {
    return optimized::run(iterations);
}

// Explicit template instantiation statements

