// original.cpp
#include "thermo.hpp"

void calcbuoyancy(double* b, double* s, double* qt, double* p, double* ql, const Grid& grid) {
    const int jj = grid.icells;
    const int kk = grid.icells * grid.jcells;
    const double thvref = 300.0; // reference virtual potential temperature
    for (int k = 0; k < grid.kcells; ++k) {
        const double exn = exner2(p[k]);
        for (int j = grid.jstart; j < grid.jend; ++j) {
            const int ij_start = j * jj;
            #pragma ivdep
            for (int i = grid.istart; i < grid.iend; ++i) {
                const int ijk = i + ij_start + k * kk;
                const double tl = s[ijk] * exn;
                ql[ij_start + i] = qt[ijk] - rslf(p[k], tl);
                ql[ij_start + i] = (ql[ij_start + i] > 0) ? calcql(s[ijk], qt[ijk], p[k], exn) : 0.0;
            }
        }
        for (int j = grid.jstart; j < grid.jend; ++j) {
            const int ij_start = j * jj;
            #pragma ivdep
            for (int i = grid.istart; i < grid.iend; ++i) {
                const int ijk = i + ij_start + k * kk;
                b[ijk] = bu(p[k], s[ijk], qt[ijk], ql[ij_start + i], thvref);
            }
        }
    }
}

// Explicit template instantiations