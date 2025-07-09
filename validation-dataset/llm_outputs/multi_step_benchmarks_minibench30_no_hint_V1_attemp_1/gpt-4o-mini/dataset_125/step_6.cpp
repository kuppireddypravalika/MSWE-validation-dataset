// header.hpp
#include <vector>
#include <string>

struct VariableEntry { int id; int addr; };

struct Module {
    std::string name;
    std::vector<std::pair<int, VariableEntry>> variables;
};

struct Serial {
    std::vector<char> buffer;
    void clear() { buffer.clear(); }
    void operator()(const std::string& s) { buffer.insert(buffer.end(), s.begin(), s.end()); buffer.push_back('\0'); }
    void operator()(size_t v) { const char* p = reinterpret_cast<const char*>(&v); buffer.insert(buffer.end(), p, p + sizeof(v)); }
    void operator()(int v) { const char* p = reinterpret_cast<const char*>(&v); buffer.insert(buffer.end(), p, p + sizeof(v)); }
};

using ModuleList = std::vector<Module*>;

void save_modules(const ModuleList&, Serial&); // implemented separately


// original.cpp
#include "header.hpp"

void save_modules(const ModuleList& modules, Serial& ar) {
    for (const auto* mod : modules) {
        ar(mod->name);
        ar(mod->variables.size());
        for (const auto& var : mod->variables) {
            ar(var.first);
            ar(var.second.addr);
        }
    }
    ar(std::string{});
}

namespace orig {
long long run(long long iterations) {
    ModuleList mods;
    mods.reserve(1000);
    for (int i = 0; i < 1000; i++) {
        auto* m = new Module{std::string("module_") + std::to_string(i), {}};
        if (i==0 || i%10==0) {
            m->variables.emplace_back(i, VariableEntry{i, i * 2});
        }
        mods.push_back(m);
    }
    Serial ar; ar.buffer.reserve(1024);
    long long dummy = 0;
    for (long long r = 0; r < iterations; r++) {
        ar.clear();
        save_modules(mods, ar);
        dummy += !ar.buffer.empty() ? ar.buffer[0] : 0;
    }
    for (auto m : mods) delete m;
    return dummy;
}
}

long long run(long long iters) { return orig::run(iters); }

// explicit template instantiations
template class std::vector<VariableEntry>;
template class std::vector<std::pair<int, VariableEntry>>;
template class std::vector<Module*>;
template class std::vector<char>;