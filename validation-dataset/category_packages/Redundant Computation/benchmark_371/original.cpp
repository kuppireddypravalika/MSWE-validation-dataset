#include "common.hpp"

// Original implementation always performs a lookup
Init* ListInit::convertInitializerTo(RecTy* Ty) const {
    if (auto* LRT = dynamic_cast<ListRecTy*>(Ty)) {
        std::vector<Init*> Elements;
        // Verify that all elements can be converted
        for (Init* I : getValues())
            if (Init* CI = I->convertInitializerTo(LRT->getElementType()))
                Elements.push_back(CI);
            else
                return nullptr;
        if (dynamic_cast<ListRecTy*>(getType()))
            return ListInit::get(Elements, Ty);
    }
    return nullptr;
}

// Helper wrapper used by the harness
ListInit* convert_list(const ListInit* li, RecTy* ty) {
    return static_cast<ListInit*>(li->convertInitializerTo(ty));
}
