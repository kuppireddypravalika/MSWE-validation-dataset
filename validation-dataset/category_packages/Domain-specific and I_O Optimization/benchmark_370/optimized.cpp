#include <Eigen/Dense>
#include <vector>
#include <cmath>

namespace {

// Direct closed-form eigenvalue computation for symmetric matrices
Eigen::Vector3d eigenvalues_direct(const Eigen::Matrix3d& A) {
    double p1 = A(0,1)*A(0,1) + A(0,2)*A(0,2) + A(1,2)*A(1,2);
    if (p1 == 0) {
        return Eigen::Vector3d(A(0,0), A(1,1), A(2,2));
    }
    double q = A.trace() / 3.0;
    double p2 = (A(0,0)-q)*(A(0,0)-q) + (A(1,1)-q)*(A(1,1)-q) + (A(2,2)-q)*(A(2,2)-q) + 2.0*p1;
    double p = std::sqrt(p2/6.0);
    Eigen::Matrix3d B = (1.0/p) * (A - q*Eigen::Matrix3d::Identity());
    double r = B.determinant() / 2.0;
    double phi;
    if (r <= -1) phi = M_PI/3.0;
    else if (r >= 1) phi = 0.0;
    else phi = std::acos(r)/3.0;
    double eig1 = q + 2.0*p*std::cos(phi);
    double eig3 = q + 2.0*p*std::cos(phi + (2.0*M_PI/3.0));
    double eig2 = 3.0*q - eig1 - eig3;
    return Eigen::Vector3d(eig1, eig2, eig3);
}

// Same helper as in original version
Eigen::Vector3d compute_vaps(const Eigen::Vector3d& tangential,
                             const Eigen::Vector3d& S_Siii,
                             const Eigen::Vector3d& S_Sii,
                             const Eigen::Vector3d&) {
    Eigen::Vector3d vap1, vap2, vap3, result;
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

    for (int j=0;j<3;++j) {
        vap1[j] = autoMat(0,j);
        vap2[j] = autoMat(1,j);
        vap3[j] = autoMat(2,j);
    }

    double n1 = vap1.norm();
    double n2 = vap2.norm();
    double n3 = vap3.norm();

    if(n1 > 1e-12) vap1 /= n1; else vap1.setZero();
    if(n2 > 1e-12) vap2 /= n2; else vap2.setZero();
    if(n3 > 1e-12) vap3 /= n3; else vap3.setZero();

    for(int l=0;l<3;++l) {
        if(vap1[2]>0.0)      result[l]=vap1[l];
        else if(vap2[2]>0.0) result[l]=vap2[l];
        else if(vap3[2]>0.0) result[l]=vap3[l];
        else                 result[l]=vap1[l];
    }
    return result;
}

} // namespace

void compute_output_step(const std::vector<Eigen::Matrix3d>& stresses,
                         std::vector<Eigen::Vector3d>& Vi_pos,
                         std::vector<Eigen::Vector3d>& Vsiii_pos) {
    const size_t N = stresses.size();
    Vi_pos.resize(N);
    Vsiii_pos.resize(N);
    for(size_t i=0;i<N;++i) {
        const Eigen::Matrix3d& stress = stresses[i];
        Eigen::Vector3d tangential(stress(1,0), stress(2,0), stress(2,1));
        Eigen::Vector3d principal = eigenvalues_direct(stress);

        Eigen::Vector3d S_Si, S_Sii, S_Siii;
        for(int j=0;j<3;++j) {
            S_Si[j]   = stress(j,j) - principal[0];
            S_Sii[j]  = stress(j,j) - principal[1];
            S_Siii[j] = stress(j,j) - principal[2];
        }

        Eigen::Vector3d Vsi   = compute_vaps(tangential, S_Siii, S_Sii, principal);
        Eigen::Vector3d Vsiii = compute_vaps(tangential, S_Sii,  S_Si,  principal);

        Vi_pos[i]    = Vsi   * principal[0];
        Vsiii_pos[i] = Vsiii * principal[2];
    }
}

