#include "header.hpp"

void save_modules(const ModuleList& modules, Serial& ar) {
    for (const auto* mod : modules) {
        ar(mod->name);
        size_t var_size = mod->variables.size(); // Store variable size once
        ar(var_size);
        for (const auto& var : mod->variables) {
            ar(var.first);
            ar(var.second.addr);
        }
    }
    ar(std::string{}); // Ensure buffer is cleared at end
}

namespace orig {
long long run(long long iterations) {
    ModuleList mods;
    mods.reserve(1000); // Pre-reserve size for efficiency
    for (int i = 0; i < 1000; ++i) {
        auto* m = new Module{std::string("module_") + std::to_string(i), {}};
        if (i % 10 == 0) { // Simplified condition to reduce checks
            m->variables.emplace_back(i, VariableEntry{i, i * 2}); // Directly emplaces VariableEntry
        }
        mods.push_back(m);
    }
    Serial ar;
    ar.buffer.reserve(1024); // Pre-reserve space for buffer
    long long dummy = 0;
    for (long long r = 0; r < iterations; ++r) {
        ar.clear(); // Clear buffer for reuse
        save_modules(mods, ar);
        dummy += !ar.buffer.empty() ? ar.buffer[0] : 0; // Use ternary for clarity
    }
    for (auto m : mods) delete m; // Clean up allocated memory
    return dummy;
}
}

long long run(long long iters) { return orig::run(iters); }

// explicit template instantiations
template class std::vector<VariableEntry>;
template class std::vector<std::pair<int, VariableEntry>>;
template class std::vector<Module*>;
template class std::vector<char>;