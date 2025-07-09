#include <vector>
#include <algorithm>

struct Monomial { int deg; }; 
struct Poly { std::vector<Monomial> coord; }; 
struct Pair { unsigned first, second; }; 
using VectPoly = std::vector<Poly>;

static double sumdegcoeffs(const VectPoly& V) {
    double r = 0;
    for (const auto& p : V)
        r += p.coord.empty() ? 0 : p.coord.front().deg;
    return r;
}

static double sumdegcoeffs2(const std::vector<VectPoly>* c, const Pair& bk) {
    return sumdegcoeffs((*c)[bk.first]) + sumdegcoeffs((*c)[bk.second]);
}

static volatile double sink;
static inline void heavy(long long loops) {
    sink = static_cast<double>(loops) * (loops - 1) * 0.00000005;
}

struct Data {
    std::vector<Pair> pairs;
    std::vector<VectPoly> coeffs;
    std::vector<int> lcms;
};

static Data generate(std::size_t N) {
    Data d;
    d.pairs.reserve(N);
    d.coeffs.resize(N);
    d.lcms.resize(N);
    for (std::size_t i = 0; i < N; ++i) {
        Poly p; p.coord.push_back({(i < N/2) ? 250 : 50});
        d.coeffs[i] = VectPoly(1, p);
        if (i + 1 < N)
            d.pairs.emplace_back(Pair{(unsigned)i, (unsigned)(i + 1)});
        d.lcms[i] = 1;
    }
    return d;
}

static Pair select_pair_original(std::vector<Pair>& B, std::vector<int>& lcms) {
    Pair bk = B.back();
    B.pop_back();
    lcms.pop_back();
    return bk;
}

static long long run_original_impl(Data d) {
    auto B = d.pairs;
    auto l = d.lcms;
    double state = 1;
    while (!B.empty()) {
        Pair bk = select_pair_original(B, l);
        double deg = sumdegcoeffs2(&d.coeffs, bk);
        long long loops = static_cast<long long>(deg * state / 50);
        heavy(loops);
        state += (deg * deg) / 200.0;
    }
    return static_cast<long long>(state);
}

long long run_algorithm(std::size_t N) {
    return run_original_impl(generate(N));
}

// Explicit template instantiation statements
