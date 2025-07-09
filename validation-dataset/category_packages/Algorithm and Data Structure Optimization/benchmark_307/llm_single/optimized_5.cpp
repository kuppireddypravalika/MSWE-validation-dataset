#include <vector>
#include <string>
#include <cstring>
#include "bench.hpp"

int run_block(int num_vars, int repeat) {
    Compiler c;
    std::vector<Variable> src(num_vars);
    std::vector<Variable> dst(num_vars);
    std::vector<int*> dst_vals(num_vars);
    long long sum = 0;

    // Create entries and initialize src
    for (int i = 0; i < num_vars; ++i) {
        src[i].name = "s" + std::to_string(i);
        src[i].create_entry(c);
        src[i].store_value(c, i);
        dst[i].name = "d" + std::to_string(i);
        dst_vals[i] = c.create_entry(dst[i].name, dst[i].type);
        dst[i].val = dst_vals[i];
    }

    for (int r = 0; r < repeat; ++r) {
        for (int i = 0; i < num_vars; ++i) {
            dst[i].store_value(c, c.insn_move_inc(c.insn_load(src[i].val)));
            sum += *dst[i].val;
        }
    }

    // Clean up dynamically allocated memory
    for (auto& v : src) delete v.val;
    for (auto& v : dst) delete v.val;

    return static_cast<int>(sum & 0x7fffffff);
}

// Explicit template instantiation
