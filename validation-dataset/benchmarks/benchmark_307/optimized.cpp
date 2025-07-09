#include <vector>
#include <string>
#include "bench.hpp"

int run_block(int num_vars, int repeat) {
    Compiler c;
    std::vector<Variable> src(num_vars), dst(num_vars);
    for (int i=0;i<num_vars;++i) {
        src[i].name = "s" + std::to_string(i);
        src[i].create_entry(c);
        src[i].store_value(c, i);
        dst[i].name = "d" + std::to_string(i);
    }
    std::vector<std::pair<Variable*,Variable*>> assigns;
    assigns.reserve(num_vars);
    for (int i=0;i<num_vars;++i) assigns.push_back({&dst[i], &src[i]});

    long long sum = 0;
    for (int r=0;r<repeat;++r) {
        for (auto& asg : assigns) {
            asg.first->create_entry(c);
            asg.first->store_value(c, c.insn_move_inc(asg.second->get_value(c)));
            sum += *asg.first->val;
        }
    }
    for (auto& v : src) delete v.val;
    for (auto& v : dst) delete v.val;
    return static_cast<int>(sum & 0x7fffffff);
}
