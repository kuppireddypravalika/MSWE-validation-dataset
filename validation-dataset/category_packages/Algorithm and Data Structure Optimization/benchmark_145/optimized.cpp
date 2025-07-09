#include <cmath>
#include <cstdint>
#include <cstdlib>

static int factorize(int n, int** fac, int** mfac) {
    *fac = (int*)calloc(n/2, sizeof(int));
    *mfac = (int*)calloc(n/2, sizeof(int));
    int d = 2, ndiv = 0;
    while (n > 1) {
        while (n % d == 0) {
            if (ndiv == 0 || (*fac)[ndiv-1] != d) {
                (*fac)[ndiv] = d;
                (*mfac)[ndiv] = 0;
                ++ndiv;
            }
            (*mfac)[ndiv-1]++;
            n /= d;
        }
        ++d;
    }
    return ndiv;
}

static int largest_divisor(int n) {
    int *div, *mdiv;
    int ndiv = factorize(n, &div, &mdiv);
    int ldiv = div[ndiv-1];
    free(div);
    free(mdiv);
    return ldiv;
}

static double heavy_compute(int loops) {
    volatile double sum = 0;
    for (int i = 0; i < loops; ++i) {
        sum += std::sqrt(static_cast<double>(i));
    }
    return sum;
}

// Optimized version uses 64-bit arithmetic to avoid overflow and skips
// the expensive computation when the condition is satisfied.
double compute(int nnodes_div, int loops) {
    long long ldiv = largest_divisor(nnodes_div);
    long long cond = ldiv * ldiv * ldiv;
    long long rhs  = static_cast<long long>(nnodes_div) * nnodes_div;
    if (cond > rhs) {
        return 0.0; // early exit, heavy computation skipped
    }
    return heavy_compute(loops);
}
