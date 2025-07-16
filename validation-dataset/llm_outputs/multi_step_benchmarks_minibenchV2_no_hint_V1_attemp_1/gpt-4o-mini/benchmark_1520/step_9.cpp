#include "distance_table.hpp"

void evaluate(SoaDistanceTableBA& table, ParticleSet& P) {
    #pragma omp parallel for
    for (int iat = 0; iat < table.Ntargets; ++iat) {
        const auto& tgt = P.R[iat];
        auto& dist = table.Distances[iat];
        auto& disp = table.Displacements[iat];
        for (int j = 0; j < table.Nsources; ++j) {
            double diff_x = table.Origin->RSoA[j][0] - tgt[0];
            double diff_y = table.Origin->RSoA[j][1] - tgt[1];
            double diff_z = table.Origin->RSoA[j][2] - tgt[2];
            disp[j] = {diff_x, diff_y, diff_z};
            dist[j] = std::sqrt(diff_x * diff_x + diff_y * diff_y + diff_z * diff_z);
        }
    }
}

// Explicit template instantiations would go here if required