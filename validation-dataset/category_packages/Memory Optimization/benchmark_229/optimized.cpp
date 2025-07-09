#include <sys/mman.h>
#include <cstdint>
#include <cstring>
#include <cstdlib>

#define VM_MAP_FAILED ((void*)-1)

static unsigned char* compiled_code = nullptr;
static size_t cache_size = 0;
static unsigned char* max_compile_start;
static unsigned char* current_compile_p;
static size_t current_cache_size;

static inline void flush_icache_hard(int) {}
static inline void write_log(const char*, ...) {}

static void* vm_acquire(size_t size) {
    return malloc(size);
}

static void vm_protect(void*, size_t, int) {}

unsigned char* alloc_cache_optimized(size_t kb) {
    cache_size = kb;
    if (compiled_code) {
        flush_icache_hard(6);
        free(compiled_code);
        compiled_code = nullptr;
    }
    if (cache_size == 0)
        return nullptr;

    while (!compiled_code && cache_size) {
        compiled_code = (unsigned char*)vm_acquire(cache_size * 1024);
        if (compiled_code == VM_MAP_FAILED || compiled_code == nullptr) {
            compiled_code = nullptr;
            cache_size /= 2;
        }
    }
    vm_protect(compiled_code, cache_size, 0);

    if (compiled_code) {
        max_compile_start = compiled_code + cache_size * 1024 - 16;
        current_compile_p = compiled_code;
        current_cache_size = 0;
    }
    return compiled_code;
}

uint64_t translation_cache_sum(int loops, size_t kb) {
    uint64_t sum = 0;
    for (int i = 0; i < loops; ++i) {
        unsigned char* p = alloc_cache_optimized(kb);
        if (!p) continue;
        memset(p, 1, kb * 1024);
        for (size_t j = 0; j < kb * 1024; ++j)
            sum += p[j];
        free(p);
        compiled_code = nullptr;
    }
    return sum;
}
