#include "thermo.hpp"

void calcbuoyancy(double* b, double* s, double* qt, double* p, double* ql, const Grid& grid) {
    int jj = grid.icells;
    int kk = grid.icells * grid.jcells;
    double thvref = 300.0; // reference virtual potential temperature
    for(int k = 0; k < grid.kcells; ++k) {
        double exn = exner2(p[k]);
        for(int j = grid.jstart; j < grid.jend; ++j) {
            int ij_start = j * jj;
            int ij = ij_start;
            #pragma ivdep
            for(int i = grid.istart; i < grid.iend; ++i) {
                int ijk = i + ij_start + k * kk;
                double tl = s[ijk] * exn;
                ql[ij] = qt[ijk] - rslf(p[k], tl);
                ql[ij] = (ql[ij] > 0) ? calcql(s[ijk], qt[ijk], p[k], exn) : 0.0;
                ij++;
            }
        }
        for(int j = grid.jstart; j < grid.jend; ++j) {
            int ij_start = j * jj;
            #pragma ivdep
            for(int i = grid.istart; i < grid.iend; ++i) {
                int ijk = i + ij_start + k * kk;
                b[ijk] = bu(p[k], s[ijk], qt[ijk], ql[i + ij_start], thvref);
            }
        }
    }
}

// Explicit template instantiations