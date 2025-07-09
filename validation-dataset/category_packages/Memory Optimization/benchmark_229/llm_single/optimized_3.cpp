#include <sys/mman.h>
#include <cstdint>
#include <cstring>
#include <cstdlib>

static unsigned char* compiled_code = nullptr;
static size_t cache_size = 0;
static unsigned char* max_compile_start;
static unsigned char* current_compile_p;
static size_t current_cache_size;

static inline void flush_icache_hard(int) {}

unsigned char* alloc_cache_original(size_t kb) {
    if (compiled_code) {
        flush_icache_hard(6);
        munmap(compiled_code, cache_size * 1024);
        compiled_code = nullptr;
    }
    if (kb == 0) return nullptr;

    cache_size = kb;
    while (!compiled_code && cache_size) {
        compiled_code = (unsigned char*)mmap(nullptr, cache_size * 1024,
                                             PROT_READ | PROT_WRITE | PROT_EXEC,
                                             MAP_PRIVATE | MAP_ANON, -1, 0);
        if (compiled_code == MAP_FAILED) {
            compiled_code = nullptr;
            cache_size /= 2;
        }
    }

    if (compiled_code) {
        max_compile_start = compiled_code + cache_size * 1024 - 16;
        current_compile_p = compiled_code;
        current_cache_size = 0;
    }
    return compiled_code;
}

uint64_t translation_cache_sum(int loops, size_t kb) {
    uint64_t sum = 0;
    size_t total_size = kb * 1024;
    unsigned char* p;

    for (int i = 0; i < loops; ++i) {
        if (!(p = alloc_cache_original(kb))) continue;
        memset(p, 1, total_size);
        sum += total_size; // each byte contributes 1 to the sum
        munmap(p, total_size);
        compiled_code = nullptr;
    }
    return sum;
}

// Explicit template instantiation (if any) follows here
