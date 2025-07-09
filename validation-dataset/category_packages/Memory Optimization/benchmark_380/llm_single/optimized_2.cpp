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
            _data = (T*) realloc(_data, _max * sizeof(T));
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
    }
    return (count * (count - 1)) / 2;
}

// Explicit template instantiations
template class Queue<unsigned>;
