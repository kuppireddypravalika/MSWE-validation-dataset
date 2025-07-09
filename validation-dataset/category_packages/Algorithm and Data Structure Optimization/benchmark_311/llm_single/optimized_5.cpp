Compiler::value pow_mpz_int(Compiler& c, std::vector<Compiler::value> args) {
    auto a_size = c.insn_call(0, {args[0]}, (void*)&mpz_log, "mpz_log");
    auto r_size = c.insn_mul(a_size, args[1]);
    auto cond = c.insn_lt(r_size, c.new_integer(10000));
    jit_label_t label_end = jit_label_undefined;
    c.insn_branch_if(cond, &label_end);

    c.insn_throw_object(1);

    jit_insn_label(c.F, &label_end);

    c.inc_ops_jit(r_size);
    Compiler::inc_mpz_counter(c.F);
    return c.insn_call(0, args, (void*)&pow_mpz_int_lambda, "pow_mpz_int");
}
