#include <mutex>
#include <cstdint>
#include <cassert>
#include <thread>
#include <vector>
#include <atomic>

class QBasicMutex {
public:
    void lock() { m.lock(); }
    void unlock() { m.unlock(); }
private:
    std::mutex m;
};

class QMutex : public QBasicMutex {};

class QMutexLocker {
public:
    explicit QMutexLocker(QBasicMutex *m) {
        assert(((reinterpret_cast<uintptr_t>(m) & uintptr_t(1u)) == uintptr_t(0)) && "QMutex pointer is misaligned");
        val = reinterpret_cast<uintptr_t>(m);
        relock();
    }
    ~QMutexLocker() { unlock(); }

    void unlock() {
        if ((val & uintptr_t(1u)) == uintptr_t(1u)) {
            val &= ~uintptr_t(1u);
            mutex()->unlock();
        }
    }

    __attribute__((noinline)) void relock() {
        if (val) {
            if ((val & uintptr_t(1u)) == uintptr_t(0u)) {
                mutex()->lock();
                val |= uintptr_t(1u);
            }
        }
    }

    QMutex *mutex() const {
        return reinterpret_cast<QMutex *>(val & ~uintptr_t(1u));
    }
private:
    uintptr_t val;
};

std::atomic<int> dummy{0};

void lock_loop(int iterations) {
    QMutex m;
    const int num_threads = std::thread::hardware_concurrency(); // Get number of hardware threads
    std::vector<std::thread> threads;

    for (int t = 0; t < num_threads; ++t) {
        threads.emplace_back([&m, iterations, t, num_threads]() {
            for (int i = t; i < iterations; i += num_threads) {
                QMutexLocker locker(&m);
                ++dummy;
            }
        });
    }

    for (auto& th : threads) {
        th.join();
    }
}

// Explicit template instantiation statements (if any) would be included here.