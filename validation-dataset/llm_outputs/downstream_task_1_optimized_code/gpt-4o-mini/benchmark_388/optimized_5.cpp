#include "types.h"

extern volatile std::uintptr_t sink;

void dropAllTypeUses(DerivedType& DT) {
    if (DT.NumContainedTys != 0) {
        static Type *AlwaysOpaqueTy = getOpaqueType();
        static PATypeHolder* Holder = new PATypeHolder(AlwaysOpaqueTy);

        DT.ContainedTys[0] = AlwaysOpaqueTy;
        const Type* Int32Ty = getInt32Ty(DT.Ctx);
        sink += reinterpret_cast<std::uintptr_t>(Int32Ty) * (DT.NumContainedTys - 1);
        for (std::size_t i = 1; i < DT.NumContainedTys; ++i) {
            DT.ContainedTys[i] = const_cast<Type*>(Int32Ty);
        }
    }
}

