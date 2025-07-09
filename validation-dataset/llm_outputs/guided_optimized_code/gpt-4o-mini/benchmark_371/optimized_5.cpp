Init* ListInit::convertInitializerTo(RecTy* Ty) const {
    // First, perform a single dynamic_cast to ListRecTy
    auto* LRT = dynamic_cast<ListRecTy*>(Ty);
    if (!LRT) {
        return nullptr; // Return early if the type does not match
    }

    std::vector<Init*> Elements;
    // Get the element type from ListRecTy once and use the pointer
    RecTy* elementType = LRT->getElementType();

    // Verify that all elements can be converted
    for (Init* I : getValues()) {
        // Perform conversion and store the result in the Elements vector
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
    return nullptr;
}


// Explicit template instantiations (if any) must be preserved.
// Please insert explicit template instantiation statements here, if applicable.