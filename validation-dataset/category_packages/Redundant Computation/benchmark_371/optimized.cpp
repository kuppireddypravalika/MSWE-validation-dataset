#include "common.hpp"

// Optimized version avoids lookup when no changes occur
Init* ListInit::convertInitializerTo(RecTy* Ty) const {
    if (getType() == Ty)
        return const_cast<ListInit*>(this);

    if (auto* LRT = dynamic_cast<ListRecTy*>(Ty)) {
        std::vector<Init*> Elements;
        Elements.reserve(getValues().size());
        bool Changed = false;
        RecTy* ElementType = LRT->getElementType();
        for (Init* I : getValues())
            if (Init* CI = I->convertInitializerTo(ElementType)) {
                Elements.push_back(CI);
                if (CI != I)
                    Changed = true;
            } else
                return nullptr;
        if (!Changed)
            return const_cast<ListInit*>(this);
        return ListInit::get(Elements, Ty);
    }
    return nullptr;
}

ListInit* convert_list(const ListInit* li, RecTy* ty) {
    return static_cast<ListInit*>(li->convertInitializerTo(ty));
}
