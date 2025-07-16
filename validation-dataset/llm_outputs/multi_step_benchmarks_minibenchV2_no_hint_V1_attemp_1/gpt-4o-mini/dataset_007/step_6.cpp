// original.cpp
#include "ripser_stub.hpp"

// Optimized enumerator using preallocated containers

template <>
class ripser<sparse_distance_matrix>::simplex_coboundary_enumerator {
    index_t idx_below, idx_above, k;
    std::vector<index_t> vertices;
    const diameter_entry_t simplex;
    const coefficient_t modulus;
    const sparse_distance_matrix& dist;
    const binomial_coeff_table& binomial_coeff;
    std::vector<std::vector<index_diameter_t>::const_reverse_iterator> neighbor_it;
    std::vector<std::vector<index_diameter_t>::const_reverse_iterator> neighbor_end;
    index_diameter_t neighbor;

public:
    simplex_coboundary_enumerator(const diameter_entry_t _simplex, const index_t _dim,
                                  const ripser& _parent)
        : idx_below(get_index(_simplex)), idx_above(0), k(_dim + 1), vertices(_dim + 1),
          simplex(_simplex), modulus(_parent.modulus), dist(_parent.dist),
          binomial_coeff(_parent.binomial_coeff) {
        _parent.get_simplex_vertices(idx_below, _dim, _parent.n, vertices.rbegin());

        // Initialize iterators for neighbors
        neighbor_it.resize(vertices.size());
        neighbor_end.resize(vertices.size());
        for (size_t i = 0; i < vertices.size(); ++i) {
            neighbor_it[i] = dist.neighbors[vertices[i]].rbegin();
            neighbor_end[i] = dist.neighbors[vertices[i]].rend();
        }
    }

    bool has_next(bool all_cofacets = true) {
        for (auto &it0 = neighbor_it[0], &end0 = neighbor_end[0]; it0 != end0; ++it0) {
            neighbor = *it0;
            for (size_t idx = 1; idx < neighbor_it.size(); ++idx) {
                auto &it = neighbor_it[idx], end = neighbor_end[idx];
                while (get_index(*it) > get_index(neighbor)) {
                    if (++it == end) return false;
                }
                if (get_index(*it) != get_index(neighbor))
                    goto continue_outer;
                else
                    neighbor = std::max(neighbor, *it);
            }
            while (k > 0 && vertices[k - 1] > get_index(neighbor)) {
                if (!all_cofacets) return false;
                idx_below -= binomial_coeff(vertices[k - 1], k);
                idx_above += binomial_coeff(vertices[k - 1], k + 1);
                --k;
            }
            return true;
        continue_outer:
        }
        return false;
    }

    diameter_entry_t next() {
        ++neighbor_it[0];
        value_t cofacet_diameter = std::max(get_diameter(simplex), get_diameter(neighbor));
        index_t cofacet_index = idx_above + binomial_coeff(get_index(neighbor), k + 1) + idx_below;
        coefficient_t cofacet_coefficient = (k & 1 ? modulus - 1 : 1) * get_coefficient(simplex) % modulus;
        return diameter_entry_t(cofacet_diameter, cofacet_index, cofacet_coefficient);
    }
};

long long run(const sparse_distance_matrix& dist, int repeat) {
    ripser<sparse_distance_matrix> r(dist);
    diameter_entry_t simplex{0.0, 0, 1};
    long long sum = 0;
    for (int i = 0; i < repeat; ++i) {
        ripser<sparse_distance_matrix>::simplex_coboundary_enumerator e(simplex, 2, r);
        while (e.has_next()) sum += get_index(e.next());
    }
    return sum;
}

// Template instantiations

template class ripser<sparse_distance_matrix>;