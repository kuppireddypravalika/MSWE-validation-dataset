#include <sys/mman.h>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <thread>
#include <vector>
#include <mutex>

static unsigned char* compiled_code = nullptr;
static size_t cache_size = 0;
static unsigned char* max_compile_start;
static unsigned char* current_compile_p;
static size_t current_cache_size;

static std::mutex mtx; // Mutex for thread safety
static uint64_t total_sum = 0; // Shared sum across threads

static inline void flush_icache_hard(int) {}

unsigned char* alloc_cache_original(size_t kb) {
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

    if (compiled_code) {
        max_compile_start = compiled_code + cache_size * 1024 - 16;
        current_compile_p = compiled_code;
        current_cache_size = 0;
    }
    return compiled_code;
}

void thread_worker(int loops, size_t kb) {
    uint64_t local_sum = 0;
    for (int i = 0; i < loops; ++i) {
        unsigned char* p = alloc_cache_original(kb);
        if (!p) continue;
        memset(p, 1, kb * 1024);
        for (size_t j = 0; j < kb * 1024; ++j)
            local_sum += p[j];
        munmap(p, kb * 1024);
        compiled_code = nullptr;
    }

    // Lock and add to the global sum
    std::lock_guard<std::mutex> lock(mtx);
    total_sum += local_sum;
}

uint64_t translation_cache_sum(int loops, size_t kb) {
    int num_threads = std::thread::hardware_concurrency(); // Get the number of hardware threads
    std::vector<std::thread> threads;
    int loops_per_thread = loops / num_threads;

    // Create threads
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(thread_worker, loops_per_thread, kb);
    }

    // Join threads
    for (std::thread &t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    return total_sum;
}

// Explicit template instantiation statements, if any.
