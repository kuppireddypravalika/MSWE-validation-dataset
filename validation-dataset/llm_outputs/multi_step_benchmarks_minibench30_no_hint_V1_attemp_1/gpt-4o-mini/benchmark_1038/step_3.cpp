#include "thermo.hpp"

void calcbuoyancy(double* b, double* s, double* qt, double* p, double* ql, const Grid& grid) {
    int jj = grid.icells;
    int kk = grid.icells * grid.jcells;
    double thvref = 300.0; // reference virtual potential temperature
    for(int k = 0; k < grid.kcells; ++k) {
        double exn = exner2(p[k]);
        for(int j = grid.jstart; j < grid.jend; ++j) {
            int ij_start = j * jj;
#pragma ivdep
            for(int i = grid.istart; i < grid.iend; ++i) {
                int ijk = i + ij_start + k * kk;
                double tl = s[ijk] * exn;
                ql[i + j * jj] = qt[ijk] - rslf(p[k], tl);
                if (ql[i + j * jj] > 0)
                    ql[i + j * jj] = calcql(s[ijk], qt[ijk], p[k], exn);
                else
                    ql[i + j * jj] = 0.0;
            }
        }
        for(int j = grid.jstart; j < grid.jend; ++j) {
            int ij_start = j * jj;
#pragma ivdep
            for(int i = grid.istart; i < grid.iend; ++i) {
                int ijk = i + ij_start + k * kk;
                b[ijk] = bu(p[k], s[ijk], qt[ijk], ql[i + j * jj], thvref);
            }
        }
    }
}

// Explicit template instantiations