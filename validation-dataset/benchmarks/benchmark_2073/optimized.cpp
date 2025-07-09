#include <cstdint>
#include <cstddef>
#include <limits>

struct BackedgeTakenInfo {
    uint64_t exact;
    uint64_t max;
};

static uint32_t countLeadingZeros(uint32_t v) {
    uint32_t c = 0;
    for (int i = 31; i >= 0; --i) {
        if (v & (1u << i)) break;
        ++c;
    }
    return c;
}

static uint32_t countLeadingOnes(uint32_t v) {
    uint32_t c = 0;
    for (int i = 31; i >= 0; --i) {
        if (!(v & (1u << i))) break;
        ++c;
    }
    return c;
}

static uint32_t GetMinLeadingZeros(uint32_t v) { return countLeadingZeros(v); }
static uint32_t GetMinSignBits(int32_t v) {
    return v < 0 ? countLeadingOnes(static_cast<uint32_t>(v))
                 : countLeadingZeros(static_cast<uint32_t>(v));
}

static BackedgeTakenInfo howManyLessThans_after(int32_t start, int32_t step, int32_t rhs, bool isSigned) {
    int32_t end = rhs;
    int32_t maxEnd = isSigned
        ? (std::numeric_limits<int32_t>::max() >> (GetMinSignBits(end) - 1))
        : (int32_t)(std::numeric_limits<uint32_t>::max() >> GetMinLeadingZeros(static_cast<uint32_t>(end)));
    uint64_t negOne = static_cast<uint64_t>(-1);
    uint64_t be = (static_cast<int64_t>(end - start) + (step + negOne)) / step;
    uint64_t maxBe = (static_cast<int64_t>(maxEnd - start) + (step + negOne)) / step;
    return {be, maxBe};
}

uint64_t run(size_t repeat, uint32_t end) {
    volatile uint64_t sum = 0;
    for (size_t r = 0; r < repeat; ++r) {
        BackedgeTakenInfo info = howManyLessThans_after(0, 1, static_cast<int32_t>(end), true);
        for (uint64_t i = 0; i < info.max; ++i) sum += (i & 1);
    }
    return sum;
}
