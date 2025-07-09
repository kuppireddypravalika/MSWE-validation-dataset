#include "common.hpp"

// Optimized implementation of convertInitializerTo
Init* ListInit::convertInitializerTo(RecTy* Ty) const {
    // Check if given type is a list type
    auto* LRT = dynamic_cast<ListRecTy*>(Ty);
    if (!LRT) return nullptr;

    size_t size = getValues().size();
    std::vector<Init*> Elements;
    Elements.reserve(size);  // Preallocate memory for vector

    // Verify all elements can be converted
    for (Init* I : getValues()) {
        Init* CI = I->convertInitializerTo(LRT->getElementType());
        if (!CI) return nullptr;
        Elements.push_back(CI);
    }

    // Only retrieve from pool if the current list type matches
    if (dynamic_cast<ListRecTy*>(getType())) {
        return ListInit::get(Elements, Ty);
    }
    return nullptr;
}

// Helper wrapper used by the harness
ListInit* convert_list(const ListInit* li, RecTy* ty) {
    return static_cast<ListInit*>(li->convertInitializerTo(ty));
}

// Explicit template instantiation
