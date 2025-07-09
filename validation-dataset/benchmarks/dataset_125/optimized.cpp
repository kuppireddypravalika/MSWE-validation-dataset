#include "header.hpp"

void save_modules(const ModuleList& modules, Serial& ar) {
    for (const auto* mod : modules) {
        if (mod->variables.empty()) {
            continue;
        }
        ar(mod->name);
        ar(mod->variables.size());
        for (const auto& var : mod->variables) {
            ar(var.first);
            ar(var.second.addr);
        }
    }
    ar(std::string{});
}

namespace opt {
long long run(long long iterations) {
    ModuleList mods;
    mods.reserve(1000);
    for (int i=0;i<1000;i++) {
        auto* m = new Module{std::string("module_")+std::to_string(i), {}};
        if (i==0 || i%10==0) {
            m->variables.push_back({i,{i,i*2}});
        }
        mods.push_back(m);
    }
    Serial ar; ar.buffer.reserve(1024);
    long long dummy=0;
    for(long long r=0;r<iterations;r++) {
        ar.clear();
        save_modules(mods, ar);
        if(!ar.buffer.empty()) dummy += ar.buffer[0];
    }
    for(auto m:mods) delete m;
    return dummy;
}
}

long long run(long long iters) { return opt::run(iters); }
