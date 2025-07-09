#include "thread_mock.h"

TThread* TThread::fgMain = nullptr;

long TThread::SelfId() { return 999; }

TThread* TThread::GetThread(long id) {
    for (TThread* th = fgMain; th; th = th->fNext)
        if (th->fId == id) return th;
    return nullptr;
}

TThread* TThread::Self() {
    static thread_local TThread* self = nullptr;
    if (!self) self = GetThread(SelfId());
    return self;
}

void init_threads(size_t n) {
    for (size_t i = 0; i < n; ++i) {
        TThread* t = new TThread(static_cast<long>(i));
        TThread::AddThread(t);
    }
}

uint64_t run(size_t iterations) {
    uint64_t sum = 0;
    for (size_t i = 0; i < iterations; ++i) {
        sum += static_cast<uint64_t>(TThread::Self()->fId);
    }
    return sum;
}
