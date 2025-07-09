#include "common.hpp"

// Optimized implementation with reduced dynamic checks
Init* ListInit::convertInitializerTo(RecTy* Ty) const {
    // Check if the target type is a ListRecTy
    auto* LRT = dynamic_cast<ListRecTy*>(Ty);
    if (!LRT) return nullptr;

    // Prepare the vector to hold converted elements
    std::vector<Init*> Elements;
    Elements.reserve(getValues().size()); // Reserve size to avoid multiple allocations

    // Verify that all elements can be converted
    for (Init* I : getValues()) {
        Init* CI = I->convertInitializerTo(LRT->getElementType());
        if (!CI) return nullptr;
        Elements.push_back(CI);
    }

    // Reuse type to avoid unnecessary dynamic allocation
    if (dynamic_cast<ListRecTy*>(getType())) {
        return ListInit::get(Elements, Ty);
    }

    return nullptr;
}

// Helper wrapper used by the harness
ListInit* convert_list(const ListInit* li, RecTy* ty) {
    return static_cast<ListInit*>(li->convertInitializerTo(ty));
}

// Explicit template instantiations (if any) should be placed here
