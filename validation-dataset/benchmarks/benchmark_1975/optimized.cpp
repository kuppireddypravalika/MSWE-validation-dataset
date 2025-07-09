#include "flatten.hpp"

bool flatten_equiv(const Expr* boe0, const Expr* boe1) {
    if (boe0->is_constant() && boe0->constant_value()) {
        return heavy_flat_exp(boe1);
    }
    if (boe1->is_constant() && boe1->constant_value()) {
        return heavy_flat_exp(boe0);
    }
    bool e0 = heavy_flat_exp(boe0);
    bool e1 = heavy_flat_exp(boe1);
    return e0 == e1;
}

extern "C" uint64_t run(size_t loops) {
    BoolLit lit(true);
    VarExpr var(false);
    uint64_t result = 0;
    for(size_t i=0;i<loops;i++) {
        var.set((i & 1) != 0);
        result += flatten_equiv(&lit, &var);
    }
    return result;
}
