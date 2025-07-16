#include "thermo.hpp"

void calcbuoyancy(double* b, double* s, double* qt, double* p, double* ql, const Grid& grid) {
    int jj = grid.icells;
    int kk = grid.icells * grid.jcells;
    double thvref = 300.0; // reference virtual potential temperature
    for(int k = 0; k < grid.kcells; ++k) {
        double exn = exner2(p[k]);
        for(int j = grid.jstart; j < grid.jend; ++j) {
#pragma ivdep
            for(int i = grid.istart; i < grid.iend; ++i) {
                int ijk = i + j * jj + k * kk;
                int ij = i + j * jj;
                double tl = s[ijk] * exn;
                ql[ij] = qt[ijk] - rslf(p[k], tl);
                if (ql[ij] > 0) {
                    ql[ij] = calcql(s[ijk], qt[ijk], p[k], exn);
                } else {
                    ql[ij] = 0.0;
                }
            }
        }
        for(int j = grid.jstart; j < grid.jend; ++j) {
#pragma ivdep
            for(int i = grid.istart; i < grid.iend; ++i) {
                int ijk = i + j * jj + k * kk;
                b[ijk] = bu(p[k], s[ijk], qt[ijk], ql[i + j * jj], thvref);
            }
        }
    }
}


// explicit template instantiations
// (if any, kept unchanged)

