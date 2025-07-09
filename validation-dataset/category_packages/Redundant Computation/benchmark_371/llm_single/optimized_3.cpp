#include "common.hpp"

// Optimized implementation of convertInitializerTo
Init* ListInit::convertInitializerTo(RecTy* Ty) const {
    if (auto* LRT = dynamic_cast<ListRecTy*>(Ty)) {
        size_t valuesSize = getValues().size();
        std::vector<Init*> Elements;
        Elements.reserve(valuesSize); // Reserve space to avoid multiple allocations

        // Using a for-loop with indices for better performance
        for (size_t i = 0; i < valuesSize; ++i) {
            Init* I = getValues()[i];
            if (Init* CI = I->convertInitializerTo(LRT->getElementType()))
                Elements.push_back(CI);
            else
                return nullptr;
        }

        // Returning the cached list if the types are compatible
        if (dynamic_cast<ListRecTy*>(getType()))
            return ListInit::get(Elements, Ty);
    }
    return nullptr;
}

// Helper wrapper used by the harness
ListInit* convert_list(const ListInit* li, RecTy* ty) {
    return static_cast<ListInit*>(li->convertInitializerTo(ty));
}

// Explicit template instantiations (reproduced unchanged)
