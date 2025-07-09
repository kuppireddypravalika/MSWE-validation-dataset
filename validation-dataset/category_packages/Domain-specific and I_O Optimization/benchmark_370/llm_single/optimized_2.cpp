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

    // Load tangential components
    const double Sxy = tangential[0];
    const double Sxz = tangential[1];
    const double Syz = tangential[2];

    // Matrix initialization
    autoMat(0,0) = S_Sii[0]*S_Siii[0] + Sxy*Sxy + Sxz*Sxz;
    autoMat(1,1) = S_Sii[1]*S_Siii[1] + Sxy*Sxy + Syz*Syz;
    autoMat(2,2) = S_Sii[2]*S_Siii[2] + Sxz*Sxz + Syz*Syz;
    autoMat(0,1) = S_Sii[0]*Sxy + Sxy*S_Siii[1] + Sxz*Syz;
    autoMat(1,0) = autoMat(0,1);
    autoMat(0,2) = S_Sii[0]*Sxz + Sxy*Syz + S_Siii[2]*Sxz;
    autoMat(2,0) = autoMat(0,2);
    autoMat(2,1) = Sxy*Sxz + S_Sii[1]*Syz + Syz*S_Siii[2];
    autoMat(1,2) = autoMat(2,1);

    // Normalize each row
    for (int j = 0; j < 3; ++j) {
        double norm = autoMat.row(j).norm();
        if (norm > 1e-12) {
            autoMat.row(j) /= norm;
        } else {
            autoMat.row(j).setZero();
        }
    }

    // Select the correct vector based on the last component
    if (autoMat(0,2) > 0.0) return autoMat.row(0);
    if (autoMat(1,2) > 0.0) return autoMat.row(1);
    return autoMat.row(2); // Default to first if all are non-positive
}

} // namespace

void compute_output_step(const std::vector<Eigen::Matrix3d>& stresses,
                         std::vector<Eigen::Vector3d>& Vi_pos,
                         std::vector<Eigen::Vector3d>& Vsiii_pos) {
    const size_t N = stresses.size();
    Vi_pos.resize(N);
    Vsiii_pos.resize(N);

    for (size_t i = 0; i < N; ++i) {
        const Eigen::Matrix3d& stress = stresses[i];
        Eigen::Vector3d tangential(stress(1,0), stress(2,0), stress(2,1));
        Eigen::Vector3d principal;
        double stress_check = stress.trace(); // Using trace for better performance

        // Compute principal values
        principal = (std::fabs(stress_check) > 1e-20) ? eigenvalues_iterative(stress) : Eigen::Vector3d::Zero();

        // Precompute shifted values in one loop for efficiency
        Eigen::Vector3d S_Si = stress.diagonal() - principal[0] * Eigen::Vector3d::Ones();
        Eigen::Vector3d S_Sii = stress.diagonal() - principal[1] * Eigen::Vector3d::Ones();
        Eigen::Vector3d S_Siii = stress.diagonal() - principal[2] * Eigen::Vector3d::Ones();

        // Compute Vsi and Vsiii
        Vi_pos[i] = compute_vaps(tangential, S_Siii, S_Sii, principal) * principal[0];
        Vsiii_pos[i] = compute_vaps(tangential, S_Sii, S_Si, principal) * principal[2];
    }
}

// Explicit template instantiation statements
