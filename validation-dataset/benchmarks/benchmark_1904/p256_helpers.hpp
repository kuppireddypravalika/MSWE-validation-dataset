#pragma once
#include <array>
#include <cstdint>

struct ProcessorInfo {
    bool supports_zNext = true;
    bool supportsArch(int) const { return supports_zNext; }
};

class CodeGenerator {
    ProcessorInfo info;
    bool vectorRegs;
public:
    explicit CodeGenerator(bool vec) : vectorRegs(vec) {}
    ProcessorInfo* getS390ProcessorInfo() { return &info; }
    bool getSupportsVectorRegisters() const { return vectorRegs; }
};

using Vec16 = std::array<uint32_t, 16>;

inline uint64_t cross_sum(const Vec16& a, const Vec16& b) {
    uint64_t sum = 0;
    for (uint32_t ai : a)
        for (uint32_t bj : b)
            sum += static_cast<uint64_t>(ai) * bj;
    return sum;
}

inline uint64_t inlineVMSL256Multiply(const Vec16& a, const Vec16& b, int iters) {
    uint64_t total = 0;
    for (int i = 0; i < iters * 10; ++i)
        total += cross_sum(a, b);
    return total / 10u;
}

inline uint64_t inlineSIMDP256Multiply(const Vec16& a, const Vec16& b, int iters) {
    uint64_t total = 0;
    for (int i = 0; i < iters; ++i)
        total += cross_sum(a, b);
    return total;
}

inline uint64_t inline256Multiply(const Vec16& a, const Vec16& b, int iters) {
    return inlineSIMDP256Multiply(a, b, iters);
}
