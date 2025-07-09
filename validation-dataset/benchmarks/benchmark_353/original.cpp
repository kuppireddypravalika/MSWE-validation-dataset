#include <pthread.h>
#include <sys/mman.h>
#include <memory>
#include <functional>
#include <stdexcept>
#include <system_error>
#include <cstddef>

// Minimal replacements for parts of Seastar used in the patch
struct attr {
    struct stack_size_type { size_t size = 0; } _stack_size{};
};

struct mmap_area {
    char* ptr = nullptr; size_t size = 0;
    mmap_area() = default;
    mmap_area(char* p, size_t s) : ptr(p), size(s) {}
    mmap_area(mmap_area&& o) noexcept : ptr(o.ptr), size(o.size) { o.ptr=nullptr; }
    mmap_area& operator=(mmap_area&& o) noexcept { ptr=o.ptr; size=o.size; o.ptr=nullptr; return *this; }
    ~mmap_area() { if(ptr) ::munmap(ptr, size); }
    char* get() const { return ptr; }
    void release() { ptr=nullptr; size=0; }
};

static mmap_area mmap_anonymous(void* addr, size_t length, int prot, int flags) {
    void* ret = ::mmap(addr, length, prot, flags | MAP_ANONYMOUS, -1, 0);
    if (ret == MAP_FAILED) throw std::system_error(errno, std::system_category());
    return mmap_area(reinterpret_cast<char*>(ret), length);
}

static char* align_up(char* p, size_t align) {
    uintptr_t v = reinterpret_cast<uintptr_t>(p);
    v = (v + align - 1) & ~(align - 1);
    return reinterpret_cast<char*>(v);
}

class posix_thread {
    std::unique_ptr<std::function<void()>> _func;
    pthread_t _pthread{};
    bool _valid = true;
    mmap_area _stack;
    static void* start_routine(void* arg) noexcept {
        auto fn = reinterpret_cast<std::function<void()>*>(arg);
        (*fn)();
        return nullptr;
    }
public:
    posix_thread(attr a, std::function<void()> func)
        : _func(std::make_unique<std::function<void()>>(std::move(func))) {
        pthread_attr_t pa; int r = pthread_attr_init(&pa); if(r) throw std::system_error(r,std::system_category());
        auto stack_size = a._stack_size.size; if(!stack_size) stack_size = 2<<20;
        _stack = mmap_anonymous(nullptr, stack_size + (4<<20), PROT_NONE, MAP_PRIVATE|MAP_NORESERVE);
        char* stack_start = align_up(_stack.get() + 1, 2<<20);
        mmap_area real_stack = mmap_anonymous(stack_start, stack_size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_STACK);
        real_stack.release();
        ::madvise(stack_start, stack_size, MADV_HUGEPAGE);
        r = pthread_attr_setstack(&pa, stack_start, stack_size); if(r) throw std::system_error(r,std::system_category());
        r = pthread_create(&_pthread, &pa, &posix_thread::start_routine, _func.get()); if(r) throw std::system_error(r,std::system_category());
    }
    ~posix_thread() { if(_valid) pthread_detach(_pthread); }
    void join() { if(_valid){ pthread_join(_pthread,nullptr); _valid=false; } }
};

size_t run(size_t loops) {
    size_t sum=0;
    for(size_t i=0;i<loops;i++) {
        posix_thread t(attr{}, []{});
        t.join();
        sum += 1;
    }
    return sum;
}

