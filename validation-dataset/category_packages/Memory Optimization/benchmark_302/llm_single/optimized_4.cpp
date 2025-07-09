#include <cstdint>
#include <cstring>
#include <cstddef>

struct APInt {
    unsigned Bits;
    unsigned Words;
    uint64_t* Data;
    APInt(unsigned bits, uint64_t val = 0) : Bits(bits) {
        Words = (bits + 63) / 64;
        Data = new uint64_t[Words];
        Data[0] = val;
        std::memset(Data + 1, 0, (Words - 1) * sizeof(uint64_t)); // Use memset for zeroing
    }
    APInt(const APInt& other) : Bits(other.Bits), Words(other.Words) {
        Data = new uint64_t[Words];
        std::memcpy(Data, other.Data, Words * sizeof(uint64_t));
    }
    ~APInt() { delete[] Data; }
    APInt& operator=(const APInt& other) {
        if (this != &other) {
            delete[] Data;
            Bits = other.Bits;
            Words = other.Words;
            Data = new uint64_t[Words];
            std::memcpy(Data, other.Data, Words * sizeof(uint64_t));
        }
        return *this;
    }
    APInt operator|(const APInt& rhs) const {
        APInt r(Bits);
        for (unsigned i = 0; i < Words; ++i) r.Data[i] = Data[i] | rhs.Data[i];
        mask(r);
        return r;
    }
    APInt operator|(uint64_t rhs) const {
        APInt r(*this);
        r.Data[0] |= rhs;
        mask(r);
        return r;
    }
    static void mask(APInt& val) {
        unsigned extra = val.Bits % 64;
        if (extra) val.Data[val.Words - 1] &= (uint64_t(-1) >> (64 - extra));
    }
    bool isAllOnesValue() const {
        for (unsigned i = 0; i + 1 < Words; ++i)
            if (Data[i] != UINT64_MAX) return false;
        uint64_t maskv = Bits % 64 ? (uint64_t(-1) >> (64 - Bits % 64)) : UINT64_MAX;
        return Data[Words - 1] == maskv;
    }
};

uint64_t run(size_t loops) {
    const unsigned Width = 256;
    uint64_t sum = 0;
    APInt knownZero(Width);
    for (size_t i = 0; i < loops; ++i) {
        knownZero.Data[0] = static_cast<uint64_t>(i);
        if ((knownZero | APInt(Width, 1)).isAllOnesValue())
            ++sum;
    }
    return sum;
}

// Explicit template instantiation statements (if any) go here
