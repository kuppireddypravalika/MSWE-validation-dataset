#include <functional>
#include <string>

namespace orig {

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

void insn_if(bool condition, const std::function<void()>& then_fn, const std::function<void()>& else_fn) {
    Label label_then = insn_init_label("then");
    Label label_else = insn_init_label("else");
    Label label_end  = insn_init_label("ifcont");

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
    auto then_fn = [&result]() { result++; }; // Use a reference directly
    for (long long i = 0; i < iterations; ++i) {
        insn_if(true, then_fn, nullptr); // Pass nullptr for else_fn directly
    }
    return result;
}

} // namespace orig

int run(long long iterations) {
    return orig::run(iterations);
}

// Explicit Template Instantiation Statements
