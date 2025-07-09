#include "types.h"

extern volatile std::uintptr_t sink;

void dropAllTypeUses(DerivedType& DT) {
    if (DT.NumContainedTys != 0) {
        static Type *AlwaysOpaqueTy = getOpaqueType();
        static PATypeHolder* Holder = new PATypeHolder(AlwaysOpaqueTy);
        DT.ContainedTys[0] = AlwaysOpaqueTy;
        Type* int32Ty = const_cast<Type*>(getInt32Ty(DT.Ctx));
        std::uintptr_t int32TyAddress = reinterpret_cast<std::uintptr_t>(int32Ty);
        for (std::size_t i = 1, e = DT.NumContainedTys; i != e; ++i) {
            DT.ContainedTys[i] = int32Ty;
            sink += int32TyAddress;
        }
    }
}

