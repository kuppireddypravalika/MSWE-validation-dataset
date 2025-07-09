#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <vector>

static int factorize(int n, std::vector<int>& fac, std::vector<int>& mfac) {
    fac.reserve(n / 2);
    mfac.reserve(n / 2);
    int d = 2, ndiv = 0;
    while (n > 1) {
        while (n % d == 0) {
            if (ndiv == 0 || fac[ndiv - 1] != d) {
                fac.push_back(d);
                mfac.push_back(0);
                ++ndiv;
            }
            ++mfac[ndiv - 1];
            n /= d;
        }
        ++d;
    }
    return ndiv;
}

static int largest_divisor(int n) {
    std::vector<int> div, mdiv;
    int ndiv = factorize(n, div, mdiv);
    return div[ndiv - 1];
}

static double heavy_compute(int loops) {
    double sum = 0.0;
    for (int i = 0; i < loops; ++i) {
        sum += std::sqrt(static_cast<double>(i));
    }
    return sum;
}

double compute(int nnodes_div, int loops) {
    int ldiv = largest_divisor(nnodes_div);
    int64_t cond = static_cast<int64_t>(ldiv) * ldiv * ldiv;
    int64_t rhs = static_cast<int64_t>(nnodes_div) * nnodes_div;
    if (cond > rhs) {
        return 0.0;
    }
    return heavy_compute(loops);
}

// Explicit template instantiation statements (if any) should be included here at the end of the file.