#include <vector>
#include <cstdint>
#include <cstddef>

struct LLVMContext {
    struct Impl { int Int32Ty; } pImpl; // minimal
};

struct Type {
    int dummy;
    __attribute__((noinline)) static const Type* getInt32Ty(LLVMContext &C) {
        // simulate some work so call overhead matters
        asm volatile("");
        return reinterpret_cast<const Type*>(&C.pImpl.Int32Ty);
    }
};

static uint64_t dropAllTypeUses_original(std::vector<const Type*>& contained, LLVMContext &C) {
    static Type AlwaysOpaque;
    contained[0] = &AlwaysOpaque;
    for (size_t i = 1; i < contained.size(); ++i)
        contained[i] = Type::getInt32Ty(C);
    uint64_t sum = 0;
    for (auto *T : contained) sum += (T != nullptr);
    return sum;
}

uint64_t run(size_t repeat, size_t elements) {
    LLVMContext C;
    std::vector<const Type*> contained(elements);
    uint64_t total = 0;
    for (size_t r = 0; r < repeat; ++r)
        total += dropAllTypeUses_original(contained, C);
    return total;
}
