#include <vector>

inline int extern_add(int a, int b) { return a + b; }

inline int fast_load2D(const std::vector<int>& arr, int width, int x, int y) {
    return arr[y * width + x];
}

long long compute_sum(const std::vector<int>& A,
                      const std::vector<int>& B,
                      int width,
                      int height) {
    long long sum = 0;
    const int total_elements = width * height;
    for (int i = 0; i < total_elements; ++i) {
        int x = i % width;
        int y = i / width;
        sum += extern_add(fast_load2D(A, width, x, y),
                           fast_load2D(B, width, x, y));
    }
    return sum;
}
