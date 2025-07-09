#include "header.hpp"

void save_modules(const ModuleList& modules, Serial& ar) {
    for (const auto* mod : modules) {
        ar(mod->name);
        size_t var_size = mod->variables.size();
        ar(var_size);
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
    Serial ar; ar.buffer.reserve(1024);

    for (int i = 0; i < 1000; i++) {
        auto* m = new Module{std::string("module_") + std::to_string(i), {}};
        if (i == 0 || i % 10 == 0) {
            m->variables.emplace_back(i, VariableEntry{i, i * 2});
        }
        mods.push_back(m);
    }

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