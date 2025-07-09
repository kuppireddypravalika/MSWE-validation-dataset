void makeNaN(APFloat& f, bool SNaN, bool Negative, const APInt& fill) {
    f.category = 1;
    f.sign = Negative;
    uint64_t* significand = f.significand.data();
    unsigned numParts = f.partCount();
    const uint64_t* fillData = fill.getRawData();
    unsigned fillWords = fill.getNumWords();

    // Efficiently set significand to fill data, zeroing out if necessary
    unsigned copyCount = std::min(numParts, fillWords);
    APIntOps::tcAssign(significand, fillData, copyCount);
    if (copyCount < numParts) {
        APIntOps::tcSet(significand + copyCount, 0, numParts - copyCount);
    }

    unsigned bitsToPreserve = f.semantics->precision - 1;
    unsigned part = bitsToPreserve / 64;
    bitsToPreserve %= 64;
    significand[part] &= ((1ULL << bitsToPreserve) - 1);

    // Clear remaining parts efficiently
    if (part + 1 < numParts) {
        std::fill(significand + part + 1, significand + numParts, 0);
    }

    unsigned QNaNBit = f.semantics->precision - 2;
    if (SNaN) {
        APIntOps::tcClearBit(significand, QNaNBit);
        if (APIntOps::tcIsZero(significand, numParts)) {
            APIntOps::tcSetBit(significand, QNaNBit - 1);
        }
    } else {
        APIntOps::tcSetBit(significand, QNaNBit);
    }
}