#include <cstdlib>
#include <cstring>

template <typename T>
class Queue {
    T* _data;
    unsigned _len;
    unsigned _max;
public:
    Queue() : _data(nullptr), _len(0), _max(16) {
        _data = (T*)malloc(_max * sizeof(T));
    }
    ~Queue() { free(_data); }
    void ensure(unsigned size) {
        if (size > _max) {
            while (_max < size) _max <<= 1;
            _data = (T*)realloc(_data, _max * sizeof(T));
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
    for (unsigned i = 0; i < count; ++i)
        q.add(i);
    unsigned long sum = (count * (count - 1)) / 2;
    return sum;
}

// Explicit template instantiations
template class Queue<unsigned>;
