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
    double sum = 0.0;
    for (int i = 0; i < loops; ++i) {
        sum += std::sqrt(static_cast<double>(i));
    }
    return sum;
}

// Optimize compute function to reduce overhead and improve performance
double compute(int nnodes_div, int loops) {
    int ldiv = largest_divisor(nnodes_div);
    int64_t cond = static_cast<int64_t>(ldiv) * ldiv * ldiv;
    int64_t rhs  = static_cast<int64_t>(nnodes_div) * nnodes_div;
    if (cond > rhs) {
        return 0.0; // would exit early, but never taken for >46k cores
    }
    return heavy_compute(loops);
}

// Explicit template instantiation statements, if any, would go here.