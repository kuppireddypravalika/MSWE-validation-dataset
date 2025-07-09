// original.cpp
#include <Eigen/Dense>
#include <vector>
#include <cmath>

namespace {

// Iterative eigenvalue computation using Eigen library
Eigen::Vector3d eigenvalues_iterative(const Eigen::Matrix3d& A) {
    Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solver(A);
    return solver.eigenvalues();
}

// Helper replicating the ComputeVaps method from the original code
Eigen::Vector3d compute_vaps(const Eigen::Vector3d& tangential,
                             const Eigen::Vector3d& S_Siii,
                             const Eigen::Vector3d& S_Sii,
                             const Eigen::Vector3d&) {
    Eigen::Vector3d result;
    Eigen::Matrix3d autoMat;

    const double Sxy = tangential[0];
    const double Sxz = tangential[1];
    const double Syz = tangential[2];

    autoMat(0,0) = S_Sii[0]*S_Siii[0] + Sxy*Sxy + Sxz*Sxz;
    autoMat(1,1) = S_Sii[1]*S_Siii[1] + Sxy*Sxy + Syz*Syz;
    autoMat(2,2) = S_Sii[2]*S_Siii[2] + Sxz*Sxz + Syz*Syz;
    autoMat(0,1) = S_Sii[0]*Sxy + Sxy*S_Siii[1] + Sxz*Syz;
    autoMat(1,0) = autoMat(0,1);
    autoMat(0,2) = S_Sii[0]*Sxz + Sxy*Syz + S_Siii[2]*Sxz;
    autoMat(2,0) = autoMat(0,2);
    autoMat(2,1) = Sxy*Sxz + S_Sii[1]*Syz + Syz*S_Siii[2];
    autoMat(1,2) = autoMat(2,1);

    Eigen::Vector3d vap1 = autoMat.row(0).normalized();
    Eigen::Vector3d vap2 = autoMat.row(1).normalized();
    Eigen::Vector3d vap3 = autoMat.row(2).normalized();

    // Select the appropriate vector based on the z-component
    result = (vap1[2] > 0.0) ? vap1 : (vap2[2] > 0.0) ? vap2 : vap3;
    return result;
}

} // namespace

// Function under test
void compute_output_step(const std::vector<Eigen::Matrix3d>& stresses,
                         std::vector<Eigen::Vector3d>& Vi_pos,
                         std::vector<Eigen::Vector3d>& Vsiii_pos) {
    const size_t N = stresses.size();
    Vi_pos.resize(N);
    Vsiii_pos.resize(N);
    for(size_t i=0;i<N;++i) {
        const Eigen::Matrix3d& stress = stresses[i];
        Eigen::Vector3d tangential(stress(1,0), stress(2,0), stress(2,1));
        Eigen::Vector3d principal;
        double stress_check = stress(0,0) + stress(1,1) + stress(2,2);
        principal = std::fabs(stress_check) > 1e-20 ? eigenvalues_iterative(stress) : Eigen::Vector3d::Zero();

        Eigen::Vector3d S_Si = stress.diagonal() - principal[0] * Eigen::Vector3d::Ones();
        Eigen::Vector3d S_Sii = stress.diagonal() - principal[1] * Eigen::Vector3d::Ones();
        Eigen::Vector3d S_Siii = stress.diagonal() - principal[2] * Eigen::Vector3d::Ones();

        Eigen::Vector3d Vsi   = compute_vaps(tangential, S_Siii, S_Sii, principal);
        Eigen::Vector3d Vsiii = compute_vaps(tangential, S_Sii,  S_Si,  principal);

        Vi_pos[i]    = Vsi   * principal[0];
        Vsiii_pos[i] = Vsiii * principal[2];
    }
}

// Explicit template instantiation statements
#include <Eigen/Core>
#include <Eigen/Geometry>
