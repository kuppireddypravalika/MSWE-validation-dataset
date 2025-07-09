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
        for (volatile int j = 0; j < 1000; ++j) {}
    }
    res->v = r;
}

__mpz_struct pow_mpz_int_lambda(__mpz_struct a, int b) {
    __mpz_struct res; mpz_init(&res); mpz_pow_ui(&res, &a, b); return res;
}

void jit_insn_branch_if(void*, void* cond_v, jit_label_t* l) {
    long cond = (long)(intptr_t)cond_v;
    for (volatile int i = 0; i < 1000; ++i) {}
    l->taken = !cond;
}

void jit_insn_label(void*, jit_label_t*) {
    for (volatile int i = 0; i < 1000; ++i) {}
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
    jit_label_t label_end = jit_label_undefined;
    jit_insn_branch_if(c.F, cond.v, &label_end);

    c.insn_throw_object(1);

    jit_insn_label(c.F, &label_end);

    c.inc_ops_jit(r_size);
    Compiler::inc_mpz_counter(c.F);
    return c.insn_call(0, args, (void*)&pow_mpz_int_lambda, "pow_mpz_int");
}
