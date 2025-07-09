#include <cstring>
#include <cctype>
#include <iostream>
#include <string>
#include <vector>

struct TClass {};

struct Root {
    TClass* FindSTLClass(const std::string& name, bool, bool) {
        // Simulate extra normalization work by directly modifying up to 1000, reduce iterations.
        std::string norm = name;
        for (int r = 0; r < 1000; ++r) {
            for (size_t i = 0; i < norm.size(); ++i) {
                norm[i] = std::tolower(norm[i]);
            }
        }
        return new TClass();
    }
    TClass* LoadClass(const std::string&, bool) { return nullptr; }
} gROOT;

struct Interpreter {
    TClass* GenerateTClass(const std::string&, bool, bool) { return new TClass(); }
} gInterpreter;

inline void normalizeToLower(std::string& str) {
    for (char& c : str) c = std::tolower(c);
}

bool IsSTL(const std::string& n) { return n.find("vector") != std::string::npos; }

TClass* GetClass(const char* name, bool load, bool silent) {
    std::string normalizedName(name);
    normalizeToLower(normalizedName);
    bool isStl = IsSTL(normalizedName);

    if (!load) return nullptr;

    TClass* cl = nullptr;
    TClass* loadedcl = gROOT.LoadClass(name, silent);
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
        if (c) {
            ++sum;
            delete c;
        }
    }
    return sum;
}

// Explicit template instantiation statements
