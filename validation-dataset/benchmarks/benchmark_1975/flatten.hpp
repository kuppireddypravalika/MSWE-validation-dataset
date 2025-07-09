#pragma once
#include <algorithm>
#include <array>
#include <random>

struct Expr {
    virtual bool eval() const = 0;
    // return true if this expression is a compile-time constant
    virtual bool is_constant() const { return false; }
    // when constant, return its value
    virtual bool constant_value() const { return false; }
    virtual ~Expr() = default;
};

struct BoolLit : Expr {
    bool value;
    explicit BoolLit(bool v) : value(v) {}
    bool eval() const override { return value; }
    bool is_constant() const override { return true; }
    bool constant_value() const override { return value; }
};

struct VarExpr : Expr {
    bool value;
    explicit VarExpr(bool v=false) : value(v) {}
    void set(bool v) { value = v; }
    bool eval() const override { return value; }
};

inline bool heavy_flat_exp(const Expr* e) {
    static std::mt19937 gen(42);
    std::array<int,1000> arr;
    for(int& v : arr) v = gen();
    std::sort(arr.begin(), arr.end());
    volatile int sum = 0;
    for(int v : arr) sum += v;
    if(sum == 123456789) return false;
    return e->eval();
}
