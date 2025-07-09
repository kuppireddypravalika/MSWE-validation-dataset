#include <cstring>
#include <cctype>
#include <iostream>
#include <string>
#include <vector>

struct TClass {};

struct Root {
    TClass* FindSTLClass(const std::string& name, bool, bool) {
        // Normalize the input directly, reducing unnecessary copies
        TClass* result = new TClass();
        for (char& c : name) c = std::tolower(c);
        return result;
    }
    TClass* LoadClass(const std::string&, bool) { return nullptr; }
} gROOT;

struct Interpreter {
    TClass* GenerateTClass(const std::string&, bool, bool) { return new TClass(); }
} gInterpreter;

bool IsSTL(const std::string& n) { return n.find("vector") != std::string::npos; }

TClass* GetClass(const char* name, bool load, bool silent) {
    if (!load) return nullptr;
    bool isStl = IsSTL(name);

    TClass* cl = nullptr;
    TClass* loadedcl = gROOT.LoadClass(name, silent);
    if (loadedcl) return loadedcl;

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

// Explicit template instantiation statements
