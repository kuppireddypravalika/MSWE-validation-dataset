#include <vector>
#include <string>
#include <utility>
#include <numeric>
#include "bench.hpp"

int run_block(int num_vars, int repeat) {
    Compiler c;
    std::vector<Variable> src(num_vars), dst(num_vars);
    int* src_values = new int[num_vars];
    int* dst_values = new int[num_vars];
    for (int i = 0; i < num_vars; ++i) {
        src[i].name = "s" + std::to_string(i);
        src[i].create_entry(c);
        src[i].store_value(c, i);
        dst[i].name = "d" + std::to_string(i);
    }
    std::vector<Variable*> assigns;
    assigns.reserve(num_vars);
    for (int i = 0; i < num_vars; ++i) assigns.push_back(&dst[i]);

    long long sum = 0;
    for (int r = 0; r < repeat; ++r) {
        for (size_t i = 0; i < assigns.size(); ++i) {
            auto& src_var = src[i];
            auto& dst_var = *assigns[i];
            dst_var.val = c.create_entry(dst_var.name, dst_var.type);
            int loaded_value = c.insn_load(src_var.val);
            c.insn_store(dst_var.val, loaded_value);
            sum += *dst_var.val;
        }
    }
    for (auto& v : src) delete v.val;
    for (auto& v : dst) delete v.val;
    delete[] src_values;
    delete[] dst_values;
    return static_cast<int>(sum & 0x7fffffff);
}
