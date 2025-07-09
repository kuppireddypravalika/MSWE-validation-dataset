#include <mutex>
#include <cstdint>
#include <cassert>

#ifndef Q_LIKELY
#define Q_LIKELY(x) (__builtin_expect(!!(x), true))
#endif

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
    explicit QMutexLocker(QBasicMutex *m)
    {
        assert(((reinterpret_cast<uintptr_t>(m) & uintptr_t(1u)) == uintptr_t(0)) && "QMutex pointer is misaligned");
        val = reinterpret_cast<uintptr_t>(m);
        if (Q_LIKELY(m)) {
            static_cast<QMutex *>(m)->lock();
            val |= 1;
        }
    }
    ~QMutexLocker() { unlock(); }

    void unlock()
    {
        if ((val & uintptr_t(1u)) == uintptr_t(1u)) {
            val &= ~uintptr_t(1u);
            mutex()->unlock();
        }
    }

    void relock()
    {
        if (val) {
            if ((val & uintptr_t(1u)) == uintptr_t(0u)) {
                mutex()->lock();
                val |= uintptr_t(1u);
            }
        }
    }

    QMutex *mutex() const
    {
        return reinterpret_cast<QMutex *>(val & ~uintptr_t(1u));
    }

private:
    uintptr_t val;
};

volatile int dummy = 0;  // ensures each build links

void lock_loop(int iterations)
{
    QMutex m;
    for (int i = 0; i < iterations; ++i) {
        QMutexLocker locker(&m);
        ++dummy;
    }
}
