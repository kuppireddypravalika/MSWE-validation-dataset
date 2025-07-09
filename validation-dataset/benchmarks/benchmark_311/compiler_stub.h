#pragma once
#include <vector>
#include <cstdint>

struct __mpz_struct { long v; };

struct jit_label_t { bool taken; };
static const jit_label_t jit_label_undefined{false};

struct Compiler {
    struct value { void* v; };
    struct label { bool taken; };
    void* F = nullptr;

    value new_integer(long n) { return {(void*)(intptr_t)n}; }
    value insn_mul(value a, value b) { long r = (long)(intptr_t)a.v * (long)(intptr_t)b.v; return {(void*)(intptr_t)r}; }
    value insn_lt(value a, value b) { long r = ((long)(intptr_t)a.v < (long)(intptr_t)b.v); return {(void*)(intptr_t)r}; }
    value insn_call(int, const std::vector<value>&, void* func, const char* name = "");
    void insn_branch_if(value cond, label* l) { l->taken = !(bool)(intptr_t)cond.v; }
    void insn_label(label* l) { (void)l; }
    void insn_throw_object(int) {}
    void inc_ops_jit(value) {}
    static void inc_mpz_counter(void*) {}
};

int mpz_log(__mpz_struct n);
void mpz_init(__mpz_struct* x);
void mpz_pow_ui(__mpz_struct* res, __mpz_struct* a, int b);
__mpz_struct pow_mpz_int_lambda(__mpz_struct a, int b);

// Slow branching helpers emulating JIT API
void jit_insn_branch_if(void*, void* cond, jit_label_t* l);
void jit_insn_label(void*, jit_label_t* l);
