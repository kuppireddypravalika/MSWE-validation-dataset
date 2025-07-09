#include "types.h"

const Type* getInt32Ty(Context& C) {
    return &C.Int32Ty;
}

Type* getOpaqueType() {
    static Type Opaque;
    return &Opaque;
}
