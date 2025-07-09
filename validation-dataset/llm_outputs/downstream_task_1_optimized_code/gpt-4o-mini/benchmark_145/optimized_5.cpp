#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <memory>

static int factorize(int n, int** fac, int** mfac) {
    int ndiv = 0;
    *fac = static_cast<int*>(std::malloc((n / 2) * sizeof(int)));
    *mfac = static_cast<int*>(std::malloc((n / 2) * sizeof(int)));
    if (!*fac || !*mfac) {
        return 0; // Handle allocation failure
    }
    int d = 2;
    while (n > 1) {
        while (n % d == 0) {
            if (ndiv == 0 || (*fac)[ndiv - 1] != d) {
                (*fac)[ndiv] = d;
                (*mfac)[ndiv] = 0;
                ++ndiv;
            }
            (*mfac)[ndiv - 1]++;
            n /= d;
        }
        ++d;
    }
    return ndiv;
}

static int largest_divisor(int n) {
    int* div;
    int* mdiv;
    int ndiv = factorize(n, &div, &mdiv);
    int ldiv = ndiv > 0 ? div[ndiv - 1] : 1; // Handle case where n=1 or other failure
    std::free(div);
    std::free(mdiv);
    return ldiv;
}

static double heavy_compute(int loops) {
    double sum = 0;
    for (int i = 0; i < loops; ++i) {
        sum += std::sqrt(static_cast<double>(i));
    }
    return sum;
}

// Original implementation using 32-bit ints; overflow prevents the check
// from triggering for large node counts, so heavy computation runs.
double compute(int nnodes_div, int loops) {
    int ldiv = largest_divisor(nnodes_div);
    auto cond = static_cast<int64_t>(ldiv) * ldiv * ldiv;
    auto rhs  = static_cast<int64_t>(nnodes_div) * nnodes_div;
    if (cond > rhs) {
        return 0.0; // would exit early, but never taken for >46k cores
    }
    return heavy_compute(loops);
}

// Explicit template instantiation statements (if any)
