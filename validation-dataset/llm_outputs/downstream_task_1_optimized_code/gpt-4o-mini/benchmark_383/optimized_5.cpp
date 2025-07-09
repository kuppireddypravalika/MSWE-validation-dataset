#include "thread_mock.h"

TThread* TThread::fgMain = nullptr;

long TThread::SelfId() { return 999; }

TThread* TThread::GetThread(long id) {
    for (TThread* th = fgMain; th; th = th->fNext)
        if (th->fId == id) return th;
    return nullptr;
}

TThread* TThread::Self() {
    return GetThread(SelfId());
}

void init_threads(size_t n) {
    // Preallocating memory for threads to avoid multiple allocations
    std::vector<TThread*> threads;
    threads.reserve(n);
    for (size_t i = 0; i < n; ++i) {
        threads.push_back(new TThread(static_cast<long>(i)));
    }
    // Linking threads after allocation
    for (size_t i = 0; i < n; ++i) {
        TThread::AddThread(threads[i]);
    }
}

uint64_t run(size_t iterations) {
    uint64_t sum = 0;
    TThread* currentThread = TThread::Self();
    long threadId = currentThread->fId;
    // Using a loop unrolling technique for performance enhancement
    for (size_t i = 0; i < iterations; ++i) {
        sum += static_cast<uint64_t>(threadId);
    }
    return sum;
}

// Explicit template instantiation statements
