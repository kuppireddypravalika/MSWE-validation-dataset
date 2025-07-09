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
    int area = width * height;
    for (int i = 0; i < area; ++i) {
        sum += extern_add(A[i], B[i]);
    }
    return sum;
}

// Explicit template instantiation statements
