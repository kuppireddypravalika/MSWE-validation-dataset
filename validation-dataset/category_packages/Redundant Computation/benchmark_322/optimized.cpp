#include <cstring>
#include <cctype>
#include <iostream>
#include <string>
#include <vector>

struct TClass {};

struct Root {
    TClass* FindSTLClass(const std::string& name, bool, bool) {
        // Simulate extra normalization work
        std::string norm = name;
        for (int r = 0; r < 1000; ++r)
            for (char& c : norm) c = std::tolower(c);
        return new TClass();
    }
    TClass* LoadClass(const std::string&, bool) { return nullptr; }
} gROOT;

struct Interpreter {
    TClass* GenerateTClass(const std::string&, bool, bool) { return new TClass(); }
} gInterpreter;

std::string heavyNormalize(const std::string& name) {
    std::string norm = name;
    for (int r = 0; r < 1000; ++r)
        for (char& c : norm) c = std::tolower(c);
    return norm;
}

bool IsSTL(const std::string& n) { return n.find("vector") != std::string::npos; }

TClass* GetClass(const char* name, bool load, bool silent) {
    std::string normalizedName = heavyNormalize(name);
    bool isStl = IsSTL(normalizedName);

    if (!load) return nullptr;

    TClass* cl = nullptr;
    TClass* loadedcl = cl ? gROOT.LoadClass("dummy", silent) : gROOT.LoadClass(name, silent);
    if (loadedcl) return loadedcl;
    if (cl) return cl;

    if (isStl) {
        return gInterpreter.GenerateTClass(normalizedName.c_str(), true, silent);
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

