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

inline void insert_page(PageMapCache& cache, uintptr_t va, uint64_t pte) {
    auto user_index = va % 1024;
    cache.user[user_index].store(pte, std::memory_order_relaxed);
    if (va < KGLOBAL) {
        cache.kernel[user_index].store(pte, std::memory_order_relaxed);
    }
}

static inline void simulate_access(std::atomic<uint64_t>& entry) {
    uint64_t v = entry.load(std::memory_order_relaxed);
    if (v & PTE_P) {
        v |= (v & PTE_A) ? 0 : PTE_A;
        v |= (v & PTE_D) ? 0 : PTE_D;
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
        if (va < KGLOBAL) {
            simulate_access(cache.kernel[va % 1024]);
        }
        sum += cache.user[va % 1024].load(std::memory_order_relaxed);
    }
    return sum;
}

// Explicit template instantiation statements if any go here.