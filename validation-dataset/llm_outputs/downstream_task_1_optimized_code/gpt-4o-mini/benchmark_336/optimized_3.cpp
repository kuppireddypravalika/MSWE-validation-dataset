#include <cstddef>
#include <vector>

struct Node { char data[10]; };

inline size_t index_from_ptr(Node* base, Node* ptr) {
    return static_cast<size_t>(ptr - base);
}

size_t run(size_t loops) {
    const size_t N = 1024;
    std::vector<Node> arr(N);
    Node* base = arr.data();
    size_t sum = 0;
    for (size_t i = 0; i < loops; ++i) {
        Node* ptr = &base[i % N];
        sum += index_from_ptr(base, ptr);
    }
    return sum;
}

// Explicit template instantiation statements, if any, should be included here.