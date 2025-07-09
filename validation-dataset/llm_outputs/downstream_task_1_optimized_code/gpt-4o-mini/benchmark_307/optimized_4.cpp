#include <vector>
#include <string>
#include <numeric>
#include <memory>
#include "bench.hpp"

int run_block(int num_vars, int repeat) {
    Compiler c;
    std::vector<Variable> src(num_vars), dst(num_vars);
    for (int i = 0; i < num_vars; ++i) {
        src[i].name = "s" + std::to_string(i);
        src[i].create_entry(c);
        src[i].store_value(c, i);
        dst[i].name = "d" + std::to_string(i);
    }

    long long sum = 0;
    for (int r = 0; r < repeat; ++r) {
        for (int i = 0; i < num_vars; ++i) {
            dst[i].val = c.create_entry(dst[i].name, dst[i].type);
            c.insn_store(dst[i].val, c.insn_move_inc(c.insn_load(src[i].val)));
            sum += *dst[i].val;
        }
    }

    for (auto& v : src) delete v.val;
    for (auto& v : dst) delete v.val;
    return static_cast<int>(sum & 0x7fffffff);
}

// Explicit template instantiation statements
