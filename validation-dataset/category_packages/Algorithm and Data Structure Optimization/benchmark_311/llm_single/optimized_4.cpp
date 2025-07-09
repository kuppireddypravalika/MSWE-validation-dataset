Compiler::value pow_mpz_int(Compiler& c, std::vector<Compiler::value> args) {
    auto a_size = c.insn_call(0, {args[0]}, (void*)&mpz_log, "mpz_log");
    auto r_size = c.insn_mul(a_size, args[1]);
    auto cond = c.insn_lt(r_size, c.new_integer(10000));

    // Create label directly without unnecessary variables
    jit_label_t label_end = {false};
    jit_insn_branch_if(c.F, cond.v, &label_end);

    // Throw the object directly if the condition is true
    c.insn_throw_object(1);

    // Labeling the end of the branch
    jit_insn_label(c.F, &label_end);

    // Increment operation count once, preserving performance
    c.inc_ops_jit(r_size);
    Compiler::inc_mpz_counter(c.F);

    // Call the power function after all checks are done
    return c.insn_call(0, args, (void*)&pow_mpz_int_lambda, "pow_mpz_int");
}
