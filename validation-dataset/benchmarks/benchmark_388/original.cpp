#include "types.h"

extern volatile std::uintptr_t sink;

void dropAllTypeUses(DerivedType& DT) {
    if (DT.NumContainedTys != 0) {
        static Type *AlwaysOpaqueTy = nullptr;
        static PATypeHolder* Holder = nullptr;
        Type *tmp = AlwaysOpaqueTy;
        if (!tmp) {
            tmp = getOpaqueType();
            PATypeHolder* tmp2 = new PATypeHolder(tmp);
            AlwaysOpaqueTy = tmp;
            Holder = tmp2;
        }
        DT.ContainedTys[0] = AlwaysOpaqueTy;
        for (std::size_t i = 1, e = DT.NumContainedTys; i != e; ++i) {
            DT.ContainedTys[i] = const_cast<Type*>(getInt32Ty(DT.Ctx));
            sink += reinterpret_cast<std::uintptr_t>(DT.ContainedTys[i]);
        }
    }
}
