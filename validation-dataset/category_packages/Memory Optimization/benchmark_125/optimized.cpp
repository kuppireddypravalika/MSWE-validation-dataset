#include <functional>
#include <string>

namespace opt {

struct Label {
    int* block;
    Label() : block(nullptr) {}
    explicit Label(int* b) : block(b) {}
    Label(Label&& o) noexcept : block(o.block) { o.block = nullptr; }
    ~Label() { delete[] block; }
    Label(const Label&) = delete;
    Label& operator=(const Label&) = delete;
};

Label insn_init_label(const std::string&) {
    return Label(new int[64]);
}

inline void insn_label(Label* l) {
    if (l->block) l->block[0]++;
}

inline void insn_branch(Label* l) { insn_label(l); }

void insn_if(bool condition,
             const std::function<void()>& then_fn,
             const std::function<void()>& else_fn) {
    auto label_then = insn_init_label("then");
    auto label_else = else_fn ? insn_init_label("else") : Label{};
    auto label_end  = insn_init_label("ifcont");

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
    std::function<void()> else_fn; // none
    for (long long i = 0; i < iterations; ++i) {
        insn_if(true, then_fn, else_fn);
    }
    return result;
}

} // namespace opt

int run(long long iterations) {
    return opt::run(iterations);
}

