#include <vector>
#include <algorithm>

struct Monomial { int deg; };
struct Poly { std::vector<Monomial> coord; };
struct Pair { unsigned first, second; };
using VectPoly = std::vector<Poly>;

static double sumdegcoeffs(const VectPoly& V) {
    double r = 0;
    for (const auto& p : V)
        r += p.coord.front().deg;
    return r;
}

static double sumdegcoeffs2(const std::vector<VectPoly>* c, const Pair& bk) {
    return sumdegcoeffs((*c)[bk.first]) + sumdegcoeffs((*c)[bk.second]);
}

static volatile double sink;
static inline void heavy(long long loops) {
    double s = 0;
    for (long long i = 0; i < loops; ++i)
        s += i * 0.0000001;
    sink = s;
}

struct Data {
    std::vector<Pair> pairs;
    std::vector<VectPoly> coeffs;
    std::vector<int> lcms;
};

static Data generate(std::size_t N) {
    Data d;
    d.pairs.reserve(N);
    d.coeffs.reserve(N);
    d.lcms.resize(N, 1);
    for (std::size_t i = 0; i < N; ++i) {
        int deg = (i < N / 2) ? 250 : 50;
        Poly p; p.coord.push_back({deg});
        d.coeffs.emplace_back(VectPoly(1, p));
        if (i + 1 < N) d.pairs.emplace_back(Pair{(unsigned)i, (unsigned)(i + 1)});
    }
    return d;
}

static std::vector<unsigned> build_smallposv(const std::vector<int>& lcms) {
    std::vector<unsigned> v(lcms.size() - 1);
    std::iota(v.begin(), v.end(), 0);
    return v;
}

static Pair select_pair_original(std::vector<Pair>& B, std::vector<int>& lcms) {
    auto small = build_smallposv(lcms);
    Pair bk = B[small.front()];
    B.erase(B.begin() + small.front());
    lcms.erase(lcms.begin() + small.front());
    return bk;
}

static long long run_original_impl(Data d) {
    auto B = std::move(d.pairs);
    auto l = std::move(d.lcms);
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

// Explicit template instantiation statements remain unchanged:
// template instantiation statements can go here
