#include <atomic>
#include <cstddef>
#include <cstdint>

constexpr uint64_t PTE_P = 0x001;
constexpr uint64_t PTE_W = 0x002;
constexpr uint64_t PTE_U = 0x004;
constexpr uint64_t PTE_A = 0x020;
constexpr uint64_t PTE_D = 0x040;
constexpr uintptr_t KGLOBAL = 1ull << 20; // 1MB

struct PageMapCache {
    std::atomic<uint64_t> user[1024];
    std::atomic<uint64_t> kernel[1024];
};

// Original implementation: does not set A/D bits on insert
inline void insert_page(PageMapCache& cache, uintptr_t va, uint64_t pte) {
    cache.user[va % 1024].store(pte, std::memory_order_relaxed);
    if (va < KGLOBAL)
        cache.kernel[va % 1024].store(pte, std::memory_order_relaxed);
}

static inline void simulate_access(std::atomic<uint64_t>& entry) {
    uint64_t v = entry.load(std::memory_order_relaxed);
    if (v & PTE_P) {
        if (!(v & PTE_A)) {
            for (volatile int i = 0; i < 100; ++i) {}
            v |= PTE_A;
        }
        if (!(v & PTE_D)) {
            for (volatile int i = 0; i < 100; ++i) {}
            v |= PTE_D;
        }
        entry.store(v, std::memory_order_relaxed);
    }
}

uint64_t run(size_t loops) {
    PageMapCache cache{};
    uint64_t sum = 0;
    for (size_t i = 0; i < loops; ++i) {
        uintptr_t va = i * 4096;
        uint64_t pte = PTE_P | PTE_W;
        insert_page(cache, va, pte);
        simulate_access(cache.user[va % 1024]);
        if (va < KGLOBAL)
            simulate_access(cache.kernel[va % 1024]);
        sum += cache.user[va % 1024].load(std::memory_order_relaxed);
    }
    return sum;
}
