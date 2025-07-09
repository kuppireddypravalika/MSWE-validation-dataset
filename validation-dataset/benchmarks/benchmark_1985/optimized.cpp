#include "tank.hpp"
#include <vector>

using state_type = std::vector<double>;

void step(const state_type& x, state_type& dxdt, double t, const TankData& tank) {
    double Va = (t > 0.0) ? tank.vin : 0.0;
    Eigen::Map<const Eigen::Matrix<double,3,1>> xvec(x.data());
    Eigen::Matrix<double,1,1> u; u << Va;
    Eigen::Map<Eigen::Matrix<double,3,1>> result(dxdt.data());
    result = tank.coeff * xvec + tank.input * u;
}
