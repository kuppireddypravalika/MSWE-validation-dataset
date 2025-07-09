#include <sys/mman.h>
#include <cstdint>
#include <cstring>
#include <cstdlib>

static unsigned char* compiled_code = nullptr;
static size_t cache_size = 0;

static inline void flush_icache_hard(int) {}

unsigned char* alloc_cache_optimized(size_t kb) {
    cache_size = kb;
    if (compiled_code) {
        flush_icache_hard(6);
        munmap(compiled_code, cache_size * 1024);
        compiled_code = nullptr;
    }
    if (cache_size == 0)
        return nullptr;

    while (!compiled_code && cache_size) {
        compiled_code = (unsigned char*)mmap(nullptr, cache_size * 1024,
                                             PROT_READ | PROT_WRITE | PROT_EXEC,
                                             MAP_PRIVATE | MAP_ANON, -1, 0);
        if (compiled_code == MAP_FAILED) {
            compiled_code = nullptr;
            cache_size /= 2;
        }
    }

    return compiled_code;
}

uint64_t translation_cache_sum(int loops, size_t kb) {
    uint64_t sum = 0;
    unsigned char* p = nullptr;
    size_t cache_size_actual = kb * 1024;

    for (int i = 0; i < loops; ++i) {
        p = alloc_cache_optimized(kb);
        if (!p) continue;

        memset(p, 1, cache_size_actual);
        sum += cache_size_actual; // As all bytes were set to 1

        munmap(p, cache_size_actual);
        compiled_code = nullptr;
    }
    return sum;
}

// Explicit template instantiation statements can be added here.
