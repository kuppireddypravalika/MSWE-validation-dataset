#include "common.hpp"

// Optimized implementation of convertInitializerTo and convert_list
Init* ListInit::convertInitializerTo(RecTy* Ty) const {
    // Check if the requested type is the same as the current type
    if (Ty == getType()) {
        return const_cast<ListInit*>(this); // Return the current instance
    }

    // Attempt to dynamically cast to ListRecTy
    ListRecTy* LRT = dynamic_cast<ListRecTy*>(Ty);
    if (!LRT) {
        return nullptr; // Return early if the type does not match
    }

    std::vector<Init*> Elements;
    // Get the element type from ListRecTy once and use the pointer
    RecTy* elementType = LRT->getElementType();

    // Verify that all elements can be converted
    for (Init* I : getValues()) {
        // Convert each initializer to the specified element type
        Init* CI = I->convertInitializerTo(elementType);
        if (!CI) {
            return nullptr; // Return if any conversion fails
        }
        Elements.push_back(CI);
    }

    // If the existing type is also a ListRecTy, lookup using the converted elements
    if (dynamic_cast<ListRecTy*>(getType())) {
        return ListInit::get(Elements, Ty);
    }
    return nullptr; // Return null if not convertible
}

// Helper wrapper for the harness
ListInit* convert_list(const ListInit* li, RecTy* ty) {
    return static_cast<ListInit*>(li->convertInitializerTo(ty));
}

// Template instantiations (unchanged)
// ...
