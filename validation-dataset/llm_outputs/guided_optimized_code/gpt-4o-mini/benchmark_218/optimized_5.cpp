#include <vector>
#include <cstdint>
#include <algorithm>
#include <stdexcept>
#include <iostream>

struct APInt {
    std::vector<uint64_t> data;
    unsigned num_words;

    APInt(unsigned total, unsigned valid) : data(total, 0), num_words(valid) {}
    unsigned getNumWords() const { return num_words; }
    const uint64_t* getRawData() const { return data.data(); }
};

namespace APIntOps {
inline void tcSet(uint64_t* dst, uint64_t val, unsigned n) {
    std::fill(dst, dst + n, val);
}

inline void tcAssign(uint64_t* dst, const uint64_t* src, unsigned n) {
    std::copy(src, src + n, dst);
}

inline void tcClearBit(uint64_t* dst, unsigned bit) {
    if (bit / 64 >= sizeof(dst) / sizeof(uint64_t)) return; // Out of bounds
    dst[bit / 64] &= ~(1ULL << (bit % 64));
}

inline void tcSetBit(uint64_t* dst, unsigned bit) {
    if (bit / 64 >= sizeof(dst) / sizeof(uint64_t)) return; // Out of bounds
    dst[bit / 64] |= (1ULL << (bit % 64));
}

inline bool tcIsZero(const uint64_t* dst, unsigned n) {
    for (unsigned i = 0; i < n; ++i) 
        if (dst[i]) return false; 
    return true;
}
}

struct FltSemantics { 
    unsigned precision; 
};

enum class FloatCategory {
    Normal,
    NaN,
    Infinity,
    Zero
};

struct APFloat {
    const FltSemantics* semantics;
    bool sign = false;
    FloatCategory category = FloatCategory::Normal;
    std::vector<uint64_t> significand;

    APFloat(const FltSemantics& sem, unsigned parts)
        : semantics(&sem), significand(parts, 0) {}

    unsigned partCount() const { return significand.size(); }
};

void makeNaN(APFloat& f, bool SNaN, bool Negative, const APInt& fill) {
    // Check if fill contains enough bits to use
    if (fill.getNumWords() < f.partCount()) {
        throw std::invalid_argument("fill does not contain enough words for significand");
    }

    // Set NaN category
    f.category = FloatCategory::NaN;
    f.sign = Negative;
    uint64_t* significand = f.significand.data();
    unsigned numParts = f.partCount();
    
    APIntOps::tcAssign(significand, fill.getRawData(), numParts);
    unsigned bitsToPreserve = f.semantics->precision - 1;

    // Ensure bitsToPreserve is within bounds
    if (bitsToPreserve > f.semantics->precision) {
        throw std::out_of_range("bitsToPreserve exceeds precision");
    }

    unsigned part = bitsToPreserve / 64;
    bitsToPreserve %= 64;

    // Clear higher bits beyond bitsToPreserve
    if (part < numParts) {
        significand[part] &= ((1ULL << bitsToPreserve) - 1);
    }

    // Clear higher parts
    for (++part; part < numParts; ++part) {
        significand[part] = 0;
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