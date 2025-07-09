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
    for (int y = 0; y < height; ++y) {
        int baseA = y * width;
        int baseB = y * width;
        for (int x = 0; x < width; ++x) {
            sum += extern_add(A[baseA + x], B[baseB + x]);
        }
    }
    return sum;
}

// Explicit template instantiation statements remain unchanged
