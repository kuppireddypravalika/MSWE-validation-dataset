#include <Eigen/Dense>
#include <vector>
#include <cmath>

namespace {

// Pre-compute constants to avoid re-allocation
const double threshold = 1e-12;
const double check_threshold = 1e-20;

Eigen::Vector3d eigenvalues_iterative(const Eigen::Matrix3d& A) {
    Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> solver(A);
    return solver.eigenvalues();
}

Eigen::Vector3d compute_vaps(const Eigen::Vector3d& tangential,
                             const Eigen::Vector3d& S_Siii,
                             const Eigen::Vector3d& S_Sii,
                             const Eigen::Vector3d&) {
    Eigen::Matrix3d autoMat;
    autoMat.diagonal() << S_Sii.array() * S_Siii.array() + tangential.squaredNorm();
    autoMat(0, 1) = S_Sii[0] * tangential[0] + tangential[0] * S_Siii[1] + tangential[1] * tangential[2];
    autoMat(1, 0) = autoMat(0, 1);
    autoMat(0, 2) = S_Sii[0] * tangential[1] + tangential[0] * tangential[2] + S_Siii[2] * tangential[1];
    autoMat(2, 0) = autoMat(0, 2);
    autoMat(1, 2) = tangential[0] * tangential[1] + S_Sii[1] * tangential[2] + tangential[2] * S_Siii[2];
    autoMat(2, 1) = autoMat(1, 2);

    Eigen::Vector3d vap1, vap2, vap3;
    for (int j=0; j<3; ++j) {
        vap1[j] = autoMat(0, j);
        vap2[j] = autoMat(1, j);
        vap3[j] = autoMat(2, j);
    }

    vap1.normalize();
    vap2.normalize();
    vap3.normalize();

    Eigen::Vector3d result = (vap1[2] > 0.0) ? vap1 : (vap2[2] > 0.0 ? vap2 : (vap3[2] > 0.0 ? vap3 : vap1));
    return result;
}

} // namespace

void compute_output_step(const std::vector<Eigen::Matrix3d>& stresses,
                         std::vector<Eigen::Vector3d>& Vi_pos,
                         std::vector<Eigen::Vector3d>& Vsiii_pos) {
    const size_t N = stresses.size();
    Vi_pos.resize(N);
    Vsiii_pos.resize(N);
    
    for (size_t i=0; i<N; ++i) {
        const Eigen::Matrix3d& stress = stresses[i];
        Eigen::Vector3d tangential(stress(1, 0), stress(2, 0), stress(2, 1));
        Eigen::Vector3d principal;
        double stress_check = stress.trace();

        if (std::fabs(stress_check) > check_threshold)
            principal = eigenvalues_iterative(stress);
        else
            principal.setZero();

        Eigen::Vector3d S_Si = stress.diagonal() - principal[0] * Eigen::Vector3d::Ones();
        Eigen::Vector3d S_Sii = stress.diagonal() - principal[1] * Eigen::Vector3d::Ones();
        Eigen::Vector3d S_Siii = stress.diagonal() - principal[2] * Eigen::Vector3d::Ones();

        Eigen::Vector3d Vsi = compute_vaps(tangential, S_Siii, S_Sii, principal);
        Eigen::Vector3d Vsiii = compute_vaps(tangential, S_Sii, S_Si, principal);

        Vi_pos[i] = Vsi * principal[0];
        Vsiii_pos[i] = Vsiii * principal[2];
    }
}

// Explicit template instantiation statements
