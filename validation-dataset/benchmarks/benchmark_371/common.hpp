#pragma once
#include <vector>
#include <cstddef>

// Minimal type system mimicking LLVM TableGen
struct RecTy { virtual ~RecTy() = default; };
struct IntRecTy : RecTy {};
struct ListRecTy : RecTy {
    RecTy* elementType;
    explicit ListRecTy(RecTy* e) : elementType(e) {}
    RecTy* getElementType() const { return elementType; }
};

struct Init {
    virtual ~Init() = default;
    virtual Init* convertInitializerTo(RecTy* Ty) const = 0;
};

struct IntInit : Init {
    int value;
    explicit IntInit(int v) : value(v) {}
    Init* convertInitializerTo(RecTy* Ty) const override {
        return dynamic_cast<IntRecTy*>(Ty) ? const_cast<IntInit*>(this) : nullptr;
    }
};

struct ListInit : Init {
    std::vector<Init*> values;
    ListRecTy* ty;
    ListInit(std::vector<Init*> v, ListRecTy* t) : values(std::move(v)), ty(t) {}
    ListRecTy* getType() const { return ty; }
    const std::vector<Init*>& getValues() const { return values; }

    static ListInit* get(const std::vector<Init*>& vals, RecTy* eltTy);
    Init* convertInitializerTo(RecTy* Ty) const override; // implemented in .cpp
};

// Global list pool mimicking FoldingSet behaviour - linear search for simplicity
inline std::vector<ListInit*>& listPool() {
    static std::vector<ListInit*> pool;
    return pool;
}

inline ListInit* ListInit::get(const std::vector<Init*>& vals, RecTy* eltTy) {
    for (auto* L : listPool()) {
        if (L->ty->getElementType() == eltTy && L->values.size() == vals.size()) {
            bool same = true;
            for (size_t i = 0; i < vals.size() && same; ++i)
                same = (L->values[i] == vals[i]);
            if (same)
                return L;
        }
    }
    auto* listTy = new ListRecTy(eltTy);
    auto* L = new ListInit(vals, listTy);
    listPool().push_back(L);
    return L;
}

