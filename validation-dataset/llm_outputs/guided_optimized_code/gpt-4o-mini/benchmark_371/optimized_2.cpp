#include "common.hpp"

// Optimized implementation of convertInitializerTo
Init* ListInit::convertInitializerTo(RecTy* Ty) const {
    // First, perform a single dynamic_cast to ListRecTy
    ListRecTy* LRT = dynamic_cast<ListRecTy*>(Ty);
    if (!LRT) {
        return nullptr; // Return early if the type does not match
    }

    std::vector<Init*> Elements;
    // Get the element type from ListRecTy once and use the pointer
    RecTy* elementType = LRT->getElementType();

    // Verify that all elements can be converted
    for (Init* I : getValues()) {
        Init* CI = I->convertInitializerTo(elementType);
        if (!CI) {
            return nullptr; // If any conversion fails, return nullptr
        }
        Elements.push_back(CI);
    }

    // If we were originally a ListRecTy, lookup using the converted elements
    if (dynamic_cast<ListRecTy*>(getType())) {
        return ListInit::get(Elements, Ty);
    }
    return nullptr; // If Ty is not ListRecTy or other checks fail, return nullptr
}

// Helper wrapper used by the harness
ListInit* convert_list(const ListInit* li, RecTy* ty) {
    return static_cast<ListInit*>(li->convertInitializerTo(ty));
}

// Explicit template instantiation statements (not provided, retain as necessary)
