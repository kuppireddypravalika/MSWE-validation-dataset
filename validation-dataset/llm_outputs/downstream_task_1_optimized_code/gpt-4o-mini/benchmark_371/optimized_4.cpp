#include "common.hpp"

// Optimized version of convertInitializerTo
Init* ListInit::convertInitializerTo(RecTy* Ty) const {
    if (auto* LRT = dynamic_cast<ListRecTy*>(Ty)) {
        std::vector<Init*> Elements;
        Elements.reserve(getValues().size());  // Reserve space to avoid multiple allocations
        // Verify that all elements can be converted
        for (Init* I : getValues()) {
            if (Init* CI = I->convertInitializerTo(LRT->getElementType())) {
                Elements.push_back(CI);
            } else {
                return nullptr;
            }
        }
        // Avoid dynamic_cast check if the type matches
        if (getType() == LRT) {
            return ListInit::get(Elements, Ty);
        }
    }
    return nullptr;
}

// Helper wrapper used by the harness
ListInit* convert_list(const ListInit* li, RecTy* ty) {
    return static_cast<ListInit*>(li->convertInitializerTo(ty));
}

// Explicit template instantiation statements

