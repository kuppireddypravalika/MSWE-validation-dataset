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
    TThread* lastThread = nullptr;
    for (size_t i = 0; i < n; ++i) {
        TThread* t = new TThread(static_cast<long>(i));
        if (lastThread) {
            lastThread->fNext = t;
        } else {
            fgMain = t;
        }
        lastThread = t;
    }
}

uint64_t run(size_t iterations) {
    TThread* self_thread = TThread::Self();
    uint64_t id = static_cast<uint64_t>(self_thread->fId);
    return id * iterations;
}

// Explicit template instantiation statements
