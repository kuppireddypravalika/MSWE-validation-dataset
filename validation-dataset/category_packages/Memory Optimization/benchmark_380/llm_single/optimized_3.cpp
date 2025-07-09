#include <cstdlib>
#include <cstring>

template <typename T>
class Queue {
    T* _data;
    unsigned _len;
    unsigned _max;
public:
    Queue() : _data(nullptr), _len(0), _max(16) { }
    ~Queue() { free(_data); }
    void ensure(unsigned size) {
        if (_max < size) {
            while (_max < size) _max <<= 1;
            T* tmp = (T*)malloc(_max * sizeof(T));
            if (_data) {
                memcpy(tmp, _data, _len * sizeof(T));
                free(_data);
            }
            _data = tmp;
        }
    }
    void add(T v) {
        ensure(_len + 1);
        _data[_len++] = v;
    }
    unsigned length() const { return _len; }
    T operator[](unsigned i) const { return _data[i]; }
};

unsigned long push_and_sum(unsigned count) {
    Queue<unsigned> q;
    unsigned long sum = 0;
    for (unsigned i = 0; i < count; ++i) {
        q.add(i);
        sum += i; // Compute sum directly in the loop
    }
    return sum;
}

// Explicit template instantiation to preserve original implementation
template class Queue<unsigned>;
