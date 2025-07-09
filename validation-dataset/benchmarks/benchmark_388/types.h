#ifndef TYPES_H
#define TYPES_H
#include <cstddef>
#include <cstdint>

struct Type { };
struct Context { Type Int32Ty; };

const Type* getInt32Ty(Context& C);

struct DerivedType {
    Context& Ctx;
    std::size_t NumContainedTys;
    Type** ContainedTys;
    DerivedType(Context& C, std::size_t N) : Ctx(C), NumContainedTys(N) {
        ContainedTys = new Type*[N];
    }
    ~DerivedType() { delete[] ContainedTys; }
};

struct PATypeHolder { Type* Ty; PATypeHolder(Type* T) : Ty(T) {} };

Type* getOpaqueType();

#endif
