#pragma once
#include <vector>
#include <utility>
#include <algorithm>
#include <cstddef>

using index_t = std::size_t;
using value_t = double;
using coefficient_t = int;

using index_diameter_t = std::pair<index_t, value_t>;
inline index_t get_index(const index_diameter_t& e) { return e.first; }
inline value_t get_diameter(const index_diameter_t& e) { return e.second; }

struct diameter_entry_t {
    value_t diameter;
    index_t index;
    coefficient_t coefficient;
    diameter_entry_t(value_t d = 0.0, index_t i = 0, coefficient_t c = 0)
        : diameter(d), index(i), coefficient(c) {}
};
inline index_t get_index(const diameter_entry_t& e) { return e.index; }
inline value_t get_diameter(const diameter_entry_t& e) { return e.diameter; }
inline coefficient_t get_coefficient(const diameter_entry_t& e) { return e.coefficient; }

struct sparse_distance_matrix {
    std::vector<std::vector<index_diameter_t>> neighbors;
    size_t size() const { return neighbors.size(); }
};

struct binomial_coeff_table {
    index_t operator()(index_t n, index_t k) const {
        if (k > n) return 0;
        if (k == 0 || k == n) return 1;
        index_t r = 1;
        for (index_t i = 1; i <= k; ++i) r = r * (n - (k - i)) / i;
        return r;
    }
};

template <typename DistanceMatrix>
class ripser {
public:
    const DistanceMatrix& dist;
    const index_t n;
    const coefficient_t modulus = 2;
    binomial_coeff_table binomial_coeff;

    ripser(const DistanceMatrix& d) : dist(d), n(d.size()) {}

    template <typename OutputIt>
    OutputIt get_simplex_vertices(index_t /*idx*/, index_t dim, index_t n_in, OutputIt out) const {
        for (index_t i = 0; i <= dim; ++i) *out++ = (n_in - i - 1);
        return out;
    }

    class simplex_coboundary_enumerator; // defined elsewhere
};

inline sparse_distance_matrix make_matrix(index_t n) {
    sparse_distance_matrix m;
    m.neighbors.resize(n);
    for (index_t i = 0; i < n; ++i) {
        for (index_t j = 0; j < n; ++j) {
            if (i == j) continue;
            m.neighbors[i].push_back({j, static_cast<value_t>(i + j)});
        }
        std::sort(m.neighbors[i].begin(), m.neighbors[i].end(),
                  [](const auto& a, const auto& b) { return get_index(a) < get_index(b); });
    }
    return m;
}
