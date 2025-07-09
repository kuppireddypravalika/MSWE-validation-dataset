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
    TThread** threads = new TThread*[n]; // Pre-allocate array for threads
    for (size_t i = 0; i < n; ++i) {
        threads[i] = new TThread(static_cast<long>(i));
    }
    for (size_t i = n; i > 0; --i) {
        TThread::AddThread(threads[i - 1]); // Add in reverse to maintain order
    }
    delete[] threads; // Free the temporary array
}

uint64_t run(size_t iterations) {
    uint64_t sum = 0;
    TThread* selfThread = TThread::Self(); // Get self thread once
    long selfId = selfThread->fId; // Store the ID for reuse
    // Use a single loop with increment
    sum = static_cast<uint64_t>(iterations) * static_cast<uint64_t>(selfId);
    return sum;
}

// Explicit template instantiation statements
