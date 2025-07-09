#include <vector>

inline int extern_add(int a, int b) { return a + b; }

inline int slow_load2D(const std::vector<int>& arr, int width, int x, int y) {
    volatile int idx = 0;
    for (int a = 0; a < y; ++a)
        for (int b = 0; b < width; ++b)
            idx++;
    for (int b = 0; b < x; ++b)
        idx++;
    return arr[idx];
}

long long compute_sum(const std::vector<int>& A,
                      const std::vector<int>& B,
                      int width,
                      int height) {
    long long sum = 0;
    for (int y = 0; y < height; ++y)
        for (int x = 0; x < width; ++x)
            sum += extern_add(slow_load2D(A, width, x, y),
                               slow_load2D(B, width, x, y));
    return sum;
}
