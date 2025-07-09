#include <vector>

inline int extern_add(int a, int b) { return a + b; }

inline int fast_load1D(const std::vector<int>& arr, int idx) {
    return arr[idx];
}

long long compute_sum(const std::vector<int>& A,
                      const std::vector<int>& B,
                      int width,
                      int height) {
    long long sum = 0;
    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x) {
            int idx = y * width + x;
            sum += extern_add(fast_load1D(A, idx), fast_load1D(B, idx));
        }
    return sum;
}
