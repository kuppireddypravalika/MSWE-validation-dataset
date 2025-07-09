#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

constexpr int GCINTERVAL = 10; // ms

static std::mutex wake_mutex;
static std::condition_variable wake_cv;
static bool stop_flag = false;
static std::vector<int> maybe_free;

// Garbage collector thread: sleeps for one second between passes
static void distref_thread() {
    while (!stop_flag) {
        std::unique_lock<std::mutex> lk(wake_mutex);
        wake_cv.wait_for(lk, std::chrono::nanoseconds(1000000000));
        maybe_free.clear();
    }
}

long long distref_simulate(int iterations) {
    stop_flag = false;
    std::thread t(distref_thread);
    long long result = 0;
    for (int i = 0; i < iterations; ++i) {
        {
            std::lock_guard<std::mutex> lk(wake_mutex);
            maybe_free.push_back(i);
            volatile int dummy = 0;
            for (int v : maybe_free)
                dummy |= v;
        }
        result += i;
    }
    stop_flag = true;
    wake_cv.notify_all();
    t.join();
    return result;
}
