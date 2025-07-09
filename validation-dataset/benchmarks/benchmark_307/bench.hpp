#include <string>

struct Compiler {
    int* create_entry(const std::string&, int) { return new int(0); }
    int insn_load(int* p) { return *p; }
    int insn_move_inc(int v) { return v; }
    void insn_store(int* p, int v) { *p = v; }
};

struct Variable {
    std::string name;
    int type{0};
    int* val{nullptr};

    void create_entry(Compiler& c) { if (!val) val = c.create_entry(name, type); }
    void store_value(Compiler& c, int v) { c.insn_store(val, v); }
    int get_value(Compiler& c) { return c.insn_load(val); }
};
