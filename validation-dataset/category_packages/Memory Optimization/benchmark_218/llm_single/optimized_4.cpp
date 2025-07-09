void makeNaN(APFloat& f, bool SNaN, bool Negative, const APInt& fill) {
    f.category = 1;
    f.sign = Negative;
    uint64_t* significand = f.significand.data();
    unsigned numParts = f.partCount();
    unsigned fillWords = fill.getNumWords();

    // Optimize by using std::min to limit unnecessary operations
    unsigned partsToFill = std::min(fillWords, numParts);
    APIntOps::tcAssign(significand, fill.getRawData(), partsToFill);

    // Clear remaining parts if fillWords is less than numParts
    if (fillWords < numParts) {
        std::fill(significand + partsToFill, significand + numParts, 0);
    }

    unsigned bitsToPreserve = f.semantics->precision - 1;
    unsigned part = bitsToPreserve / 64;
    bitsToPreserve %= 64;
    significand[part] &= ((1ULL << bitsToPreserve) - 1);

    unsigned QNaNBit = f.semantics->precision - 2;
    // Consolidate bit manipulation logic
    if (SNaN) {
        APIntOps::tcClearBit(significand, QNaNBit);
        if (APIntOps::tcIsZero(significand, numParts)) {
            APIntOps::tcSetBit(significand, QNaNBit - 1);
        }
    } else {
        APIntOps::tcSetBit(significand, QNaNBit);
    }
}
