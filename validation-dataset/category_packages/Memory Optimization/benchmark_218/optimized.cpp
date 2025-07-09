#include <vector>
#include <cstdint>
#include <algorithm>

struct APInt {
    std::vector<uint64_t> data;
    unsigned num_words;
    APInt(unsigned total, unsigned valid) : data(total, 0), num_words(valid) {}
    unsigned getNumWords() const { return num_words; }
    const uint64_t* getRawData() const { return data.data(); }
};

namespace APIntOps {
inline void tcSet(uint64_t* dst, uint64_t val, unsigned n) {
    for (unsigned i = 0; i < n; ++i) dst[i] = val;
}
inline void tcAssign(uint64_t* dst, const uint64_t* src, unsigned n) {
    std::copy(src, src + n, dst);
}
inline void tcClearBit(uint64_t* dst, unsigned bit) {
    dst[bit / 64] &= ~(1ULL << (bit % 64));
}
inline void tcSetBit(uint64_t* dst, unsigned bit) {
    dst[bit / 64] |= (1ULL << (bit % 64));
}
inline bool tcIsZero(const uint64_t* dst, unsigned n) {
    for (unsigned i = 0; i < n; ++i) if (dst[i]) return false; return true;
}
}

struct FltSemantics { unsigned precision; };

struct APFloat {
    const FltSemantics* semantics;
    bool sign = false;
    unsigned category = 0;
    std::vector<uint64_t> significand;
    APFloat(const FltSemantics& sem, unsigned parts)
        : semantics(&sem), significand(parts, 0) {}
    unsigned partCount() const { return significand.size(); }
};

void makeNaN(APFloat& f, bool SNaN, bool Negative, const APInt& fill) {
    f.category = 1;
    f.sign = Negative;
    uint64_t* significand = f.significand.data();
    unsigned numParts = f.partCount();
    if (fill.getNumWords() < numParts)
        APIntOps::tcSet(significand, 0, numParts);
    APIntOps::tcAssign(significand, fill.getRawData(),
                       std::min(fill.getNumWords(), numParts));
    unsigned bitsToPreserve = f.semantics->precision - 1;
    unsigned part = bitsToPreserve / 64;
    bitsToPreserve %= 64;
    significand[part] &= ((1ULL << bitsToPreserve) - 1);
    for (part++; part != numParts; ++part)
        significand[part] = 0;
    unsigned QNaNBit = f.semantics->precision - 2;
    if (SNaN) {
        APIntOps::tcClearBit(significand, QNaNBit);
        if (APIntOps::tcIsZero(significand, numParts))
            APIntOps::tcSetBit(significand, QNaNBit - 1);
    } else {
        APIntOps::tcSetBit(significand, QNaNBit);
    }
}
