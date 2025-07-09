#include <cstddef>
#include <vector>

struct Node { char data[10]; };

inline size_t index_from_ptr(Node* base, Node* ptr, volatile size_t elem_size) {
    // Compute pointer distance manually in bytes then divide by element size
    char* b = reinterpret_cast<char*>(base);
    char* p = reinterpret_cast<char*>(ptr);
    size_t distance = p - b;
    return distance / elem_size;
}

size_t run(size_t loops) {
    const size_t N = 1024;
    std::vector<Node> arr(N);
    volatile size_t elem_size = sizeof(Node); // prevent compile-time constant
    size_t sum = 0;
    for (size_t i = 0; i < loops; ++i) {
        Node* ptr = &arr[i % N];
        sum += index_from_ptr(arr.data(), ptr, elem_size);
    }
    return sum;
}
