#include <cstdlib>
#include <cstring>
#include <algorithm>

template <typename T>
class Queue {
    T* _data{};
    unsigned _len{};
    unsigned _max{};
public:
    ~Queue() { free(_data); }
    void ensure(unsigned size) {
        if (_max > size)
            return;
        if (!_max)
            _max = 8;
        _max = std::max(_max * 2u, size);
        T* tmp = (T*)malloc(_max * sizeof(T));
        if (_data)
            memcpy(tmp, _data, _len * sizeof(T));
        free(_data);
        _data = tmp;
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
    unsigned long sum = 0;
    for (unsigned i = 0; i < q.length(); ++i)
        sum += q[i];
    return sum;
}
