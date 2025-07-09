#pragma once
#include <algorithm>
#include <cmath>
#include <vector>

struct Grid {
    int icells;
    int jcells;
    int kcells;
    int istart;
    int iend;
    int jstart;
    int jend;
};

constexpr double grav = 9.81;
constexpr double lv = 2.5e6;
constexpr double cp = 1004.0;
constexpr double rv = 461.0;
constexpr double rd = 287.0;
constexpr double p0 = 100000.0;
constexpr double ep = rv/rd;
constexpr double tmelt = 273.15;

inline double exner2(double p) {
    double dp = p - p0;
    return 1.0 + dp*(1e-5 + dp*(-2e-11 + dp*(3e-17 + dp*(-4e-23 + dp*(5e-29 + dp*(6e-35 + 7e-41*dp))))));
}

inline double esl(double t) {
    double x = std::max(-80.0, t - tmelt);
    return 1.0e3 + x*(1e-1 + x*(1e-3 + x*(1e-5 + x*(1e-7 + x*(1e-9 + x*(1e-11 + x*1e-13))))));
}

inline double rslf(double p, double t) {
    double e = esl(t);
    return ep * e / (p - (1 - ep) * e);
}

inline double calcql(double s, double qt, double p, double exn) {
    double tl = s * exn;
    double tnr_old = 1e9;
    double tnr = tl;
    while (std::fabs(tnr - tnr_old)/tnr_old > 1e-5) {
        tnr_old = tnr;
        double qs = rslf(p, tnr);
        tnr = tnr - (tnr + (lv/cp)*qs - tl - (lv/cp)*qt) /
                      (1 + (lv*lv*qs)/(rv*cp*tnr*tnr));
    }
    double qs = rslf(p, tnr);
    return std::max(0.0, qt - qs);
}

inline double bu(double p, double s, double qt, double ql, double thvref) {
    return grav * ((s + lv*ql/(cp*exner2(p))) * (1 - (1 - rv/rd)*qt - rv/rd*ql) - thvref) / thvref;
}

void calcbuoyancy(double* b, double* s, double* qt, double* p, double* ql, const Grid& grid);
