#include "common.hpp"

// Optimized implementation with reduced dynamic casting and better memory usage
Init* ListInit::convertInitializerTo(RecTy* Ty) const {
    if (auto* LRT = dynamic_cast<ListRecTy*>(Ty)) {
        std::vector<Init*> Elements;
        Elements.reserve(getValues().size()); // Reserve to avoid multiple allocations
        // Verify that all elements can be converted
        for (Init* I : getValues()) {
            if (Init* CI = I->convertInitializerTo(LRT->getElementType())) {
                Elements.push_back(CI);
            } else {
                return nullptr;
            }
        }
        // Avoid redundant type checking and directly return the list if it's the same type
        if (getType() != LRT) {
            return ListInit::get(Elements, Ty);
        }
        return this; // Return this instance if it's the same type
    }
    return nullptr;
}

// Helper wrapper used by the harness
ListInit* convert_list(const ListInit* li, RecTy* ty) {
    return static_cast<ListInit*>(li->convertInitializerTo(ty));
}

// Explicit template instantiation statements from original code
