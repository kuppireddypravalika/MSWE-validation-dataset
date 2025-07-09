#include "tank.hpp"
#include <vector>

using state_type = std::vector<double>;

void step(const state_type& x, state_type& dxdt, double t, const TankData& tank) {
    double Va = (t > 0.0) ? tank.vin : 0.0;
    Eigen::Map<const Eigen::Matrix<double,3,1>> xvec(x.data());
    Eigen::Matrix<double,1,1> u; u << Va;
    for (size_t nodeno = 0; nodeno <= 2; ++nodeno) {
        double sum = 0.0;
        for (int j = 0; j < 3; ++j) sum += tank.coeff(nodeno, j) * x.at(j);
        for (int j = 0; j < 1; ++j) sum += tank.input(nodeno, j) * u(j);
        dxdt.at(nodeno) = sum;
    }
}
