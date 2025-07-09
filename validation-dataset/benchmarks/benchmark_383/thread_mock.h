#ifndef THREAD_MOCK_H
#define THREAD_MOCK_H
#include <cstddef>
#include <cstdint>

struct TThread {
    long fId;
    TThread* fNext;
    TThread(long id) : fId(id), fNext(nullptr) {}
    static TThread* fgMain;
    static void AddThread(TThread* th) {
        th->fNext = fgMain;
        fgMain = th;
    }
    static long SelfId();
    static TThread* GetThread(long id);
    static TThread* Self();
};

void init_threads(size_t n);
uint64_t run(size_t iterations);

#endif
