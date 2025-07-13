#include <iostream>
#include <vector>
#include <random>
#include "agent_optimized.cpp"

int main() {
    constexpr size_t N = 1 << 20; // 1 million elements
    std::vector<unsigned int> src(N);
    std::vector<unsigned int> dst(N);

    // Initialize src with random data
    std::mt19937 rng(12345);
    std::uniform_int_distribution<unsigned int> dist(0, 1 << 16);
    for (size_t i = 0; i < N; ++i) {
        src[i] = dist(rng);
    }

    // Print first 10 elements of src before sorting
    std::cout << "src before sorting: ";
    for (size_t i = 0; i < 10; ++i) {
        std::cout << src[i] << " ";
    }
    std::cout << std::endl;

    // Run the optimized sort
    run_sort(src.data(), dst.data(), N);

    // Print first 10 elements of dst after sorting
    std::cout << "dst after sorting: ";
    for (size_t i = 0; i < 10; ++i) {
        std::cout << dst[i] << " ";
    }
    std::cout << std::endl;

    // Verify the result (simple check)
    bool sorted = true;
    for (size_t i = 1; i < N; ++i) {
        if (dst[i - 1] > dst[i]) {
            sorted = false;
            break;
        }
    }

    if (sorted) {
        std::cout << "Sorting successful!" << std::endl;
    } else {
        std::cout << "Sorting failed!" << std::endl;
    }

    return 0;
}
// Enable vectorization and parallel processing
void radix_iter0(volatile unsigned int* src, unsigned int* radixCount,
                 size_t N, size_t shift, size_t threadIndex) {
    size_t startID = threadIndex * N / THREADS;
    size_t endID   = (threadIndex + 1) * N / THREADS;
    const unsigned int mask = BUCKETS - 1;
    #pragma omp simd
    for (size_t i = 0; i < BUCKETS; ++i)
        radixCount[threadIndex * BUCKETS + i] = 0;
    for (size_t i = startID; i < endID; ++i) {
        unsigned int index = (src[i] >> shift) & mask;
        radixCount[threadIndex * BUCKETS + index]++;
    }
    // Debug print
    std::cout << "radix_iter0 - thread " << threadIndex << ": ";
    for (size_t i = 0; i < BUCKETS; ++i) {
        std::cout << radixCount[threadIndex * BUCKETS + i] << " ";
    }
    std::cout << std::endl;
}

// Enable vectorization and parallel processing
void radix_iter1(volatile unsigned int* src, unsigned int* dst, unsigned int* radixCount,
                 size_t N, size_t shift, size_t threadIndex, size_t threadCount) {
    size_t startID = threadIndex * N / threadCount;
    size_t endID   = (threadIndex + 1) * N / threadCount;
    const unsigned int mask = BUCKETS - 1;

    alignas(64) unsigned int total[BUCKETS];
    #pragma omp simd
    for (size_t i = 0; i < BUCKETS; ++i)
        total[i] = 0;
    for (size_t i = 0; i < threadCount; ++i)
        for (size_t j = 0; j < BUCKETS; ++j)
            total[j] += radixCount[i * BUCKETS + j];

    alignas(64) unsigned int offset[BUCKETS];
    offset[0] = 0;
    for (size_t i = 1; i < BUCKETS; ++i)
        offset[i] = offset[i - 1] + total[i - 1];

    for (size_t i = 0; i < threadIndex; ++i)
        for (size_t j = 0; i < BUCKETS; ++j)
            offset[j] += radixCount[i * BUCKETS + j];

    for (size_t i = startID; i < endID; ++i) {
        unsigned int elt = src[i];
        unsigned int index = (elt >> shift) & mask;
        dst[offset[index]++] = elt;
    }
    // Debug print
    std::cout << "radix_iter1 - thread " << threadIndex << ": ";
    for (size_t i = 0; i < BUCKETS; ++i) {
        std::cout << offset[i] << " ";
    }
    std::cout << std::endl;
}

// Entry point used by the harness
void run_sort(volatile unsigned int* src, unsigned int* dst, size_t N) {
    std::vector<unsigned int> counts(THREADS * BUCKETS);
    #pragma omp parallel for
    for (size_t t = 0; t < THREADS; ++t)
        radix_iter0(src, counts.data(), N, 0, t);
    #pragma omp parallel for
    for (size_t t = 0; t < THREADS; ++t)
        radix_iter1(src, dst, counts.data(), N, 0, t, THREADS);
}
