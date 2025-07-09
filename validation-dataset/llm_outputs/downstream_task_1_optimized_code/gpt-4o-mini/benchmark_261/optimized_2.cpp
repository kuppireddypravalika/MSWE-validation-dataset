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
    explicit QMutexLocker(QBasicMutex *m) : mutex_(m) {
        assert(((reinterpret_cast<uintptr_t>(m) & uintptr_t(1u)) == uintptr_t(0)) && "QMutex pointer is misaligned");
        relock();
    }
    ~QMutexLocker() { unlock(); }

    void unlock() {
        if (locked_) {
            locked_ = false;
            mutex()->unlock();
        }
    }

    __attribute__((noinline)) void relock() {
        if (mutex_ && !locked_) {
            mutex()->lock();
            locked_ = true;
        }
    }

    QMutex *mutex() const { return reinterpret_cast<QMutex *>(reinterpret_cast<uintptr_t>(mutex_)); }

private:
    QBasicMutex* mutex_;
    bool locked_ = false;
};

std::atomic<int> dummy(0);

void lock_loop(int iterations) {
    QMutex m;
    std::vector<std::thread> threads;
    const int thread_count = std::thread::hardware_concurrency();
    const int iterations_per_thread = iterations / thread_count;

    for (int j = 0; j < thread_count; ++j) {
        threads.emplace_back([=, &m]() {
            for (int i = 0; i < iterations_per_thread; ++i) {
                QMutexLocker locker(&m);
                ++dummy;
            }
        });
    }

    for (auto &th : threads) {
        th.join();
    }
}
