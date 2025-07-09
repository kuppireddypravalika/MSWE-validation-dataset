#include "compiler_stub.h"
#include <cmath>

int mpz_log(__mpz_struct n) {
    long val = n.v;
    int r = 0;
    while (val > 1) { val >>= 1; ++r; }
    return r + 1;
}

void mpz_init(__mpz_struct* x) { x->v = 0; }

void mpz_pow_ui(__mpz_struct* res, __mpz_struct* a, int b) {
    long r = 1;
    for (int i = 0; i < b; ++i) {
        r *= a->v;
    }
    res->v = r;
}

__mpz_struct pow_mpz_int_lambda(__mpz_struct a, int b) {
    __mpz_struct res; mpz_init(&res); mpz_pow_ui(&res, &a, b); return res;
}

Compiler::value Compiler::insn_call(int, const std::vector<value>& args, void* func, const char*) {
    if (func == (void*)(&mpz_log)) {
        __mpz_struct* a = (__mpz_struct*)args[0].v;
        long r = mpz_log(*a);
        return {(void*)(intptr_t)r};
    } else {
        __mpz_struct* a = (__mpz_struct*)args[0].v;
        long b = (long)(intptr_t)args[1].v;
        __mpz_struct* res = new __mpz_struct(pow_mpz_int_lambda(*a, b));
        return {res};
    }
}

Compiler::value pow_mpz_int(Compiler& c, std::vector<Compiler::value> args) {
    auto a_size = c.insn_call(0, {args[0]}, (void*)&mpz_log, "mpz_log");
    auto r_size = c.insn_mul(a_size, args[1]);
    auto cond = c.insn_lt(r_size, c.new_integer(10000));
    Compiler::label label_end;
    c.insn_branch_if(cond, &label_end);

    c.insn_throw_object(1);

    c.insn_label(&label_end);

    c.inc_ops_jit(r_size);
    Compiler::inc_mpz_counter(c.F);
    return c.insn_call(0, args, (void*)&pow_mpz_int_lambda, "pow_mpz_int");
}
