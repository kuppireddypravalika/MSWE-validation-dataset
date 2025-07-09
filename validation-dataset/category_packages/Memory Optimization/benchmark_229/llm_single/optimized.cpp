#include <sys/mman.h>
#include <cstdint>
#include <cstring>
#include <cstdlib>

static unsigned char* compiled_code = nullptr;
static size_t cache_size = 0;

static inline void flush_icache_hard(int) {}

unsigned char* alloc_cache_original(size_t kb) {
    if (compiled_code) {
        flush_icache_hard(6);
        munmap(compiled_code, cache_size * 1024);
        compiled_code = nullptr;
    }
    if (kb == 0) return nullptr;
    cache_size = kb;

    while (cache_size) {
        compiled_code = (unsigned char*)mmap(nullptr, cache_size * 1024,
                                             PROT_READ | PROT_WRITE | PROT_EXEC,
                                             MAP_PRIVATE | MAP_ANON, -1, 0);
        if (compiled_code != MAP_FAILED) break;
        cache_size /= 2;
    }

    return compiled_code;
}

uint64_t translation_cache_sum(int loops, size_t kb) {
    uint64_t sum = 0;
    size_t alloc_size = kb * 1024;
    for (int i = 0; i < loops; ++i) {
        unsigned char* p = alloc_cache_original(kb);
        if (!p) continue;
        memset(p, 1, alloc_size);
        sum += alloc_size; // Everyone's set to 1; thus total is size
        munmap(p, alloc_size);
        compiled_code = nullptr;
    }
    return sum;
}
