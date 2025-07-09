#pragma once
#include <Eigen/Dense>

using Eigen::Matrix3d;

struct TankData {
    double vin;
    Matrix3d coeff;
    Eigen::Matrix<double,3,1> input;
};

inline TankData create_tank() {
    TankData t;
    t.vin = 3.3;
    Matrix3d Cprime = Matrix3d::Identity();
    Matrix3d Gprime;
    Gprime << 4,1,2,
              1,5,3,
              2,3,6;
    t.coeff = Cprime.ldlt().solve(-1.0 * Gprime);
    t.input = Cprime.ldlt().solve(Eigen::Matrix<double,3,1>::Ones());
    return t;
}
