#include <cstring>
#include <cctype>
#include <iostream>
#include <string>
#include <vector>

struct TClass {};

struct Root {
    TClass* FindSTLClass(const std::string& name, bool, bool) {
        // Simulate extra normalization work
        TClass* tClass = new TClass();
        return tClass;
    }
    TClass* LoadClass(const std::string&, bool) { return nullptr; }
} gROOT;

struct Interpreter {
    TClass* GenerateTClass(const std::string&, bool, bool) { return new TClass(); }
} gInterpreter;

inline std::string heavyNormalize(const std::string& name) {
    std::string norm(name);
    for (int r = 0; r < 1000; ++r)
        for (char& c : norm) c = std::tolower(c);
    return norm;
}

inline bool IsSTL(const std::string& n) { return n.find("vector") != std::string::npos; }

TClass* GetClass(const char* name, bool load, bool silent) {
    if (!load) return nullptr;

    std::string normalizedName = heavyNormalize(name);
    bool isStl = IsSTL(normalizedName);

    TClass* cl = nullptr;

    TClass* loadedcl = gROOT.LoadClass(cl ? "dummy" : name, silent);
    if (loadedcl) return loadedcl;
    if (cl) return cl;

    if (isStl) {
        return gROOT.FindSTLClass(name, true, silent);
    } else if (std::strncmp(name, "std::", 5) == 0) {
        return gInterpreter.GenerateTClass(name + 5, true, silent);
    }

    return gInterpreter.GenerateTClass(name, false, silent);
}

size_t run(size_t loops) {
    size_t sum = 0;
    for (size_t i = 0; i < loops; ++i) {
        TClass* c = GetClass("std::vector<int>", true, false);
        if (c) ++sum;
        delete c;
    }
    return sum;
}

// Explicit template instantiation statements (if needed) go here.
