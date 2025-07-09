#include "types.h"

extern volatile std::uintptr_t sink;

void dropAllTypeUses(DerivedType& DT) {
    if (DT.NumContainedTys != 0) {
        static Type *AlwaysOpaqueTy = getOpaqueType();
        static PATypeHolder* Holder = new PATypeHolder(AlwaysOpaqueTy);

        DT.ContainedTys[0] = AlwaysOpaqueTy;
        const Type* int32Ty = getInt32Ty(DT.Ctx);

        for (std::size_t i = 1; i < DT.NumContainedTys; ++i) {
            DT.ContainedTys[i] = const_cast<Type*>(int32Ty);
            sink += reinterpret_cast<std::uintptr_t>(DT.ContainedTys[i]);
        }
    }
}
