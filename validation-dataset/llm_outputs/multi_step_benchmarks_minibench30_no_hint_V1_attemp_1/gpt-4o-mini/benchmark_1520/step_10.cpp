#include "distance_table.hpp"

void evaluate(SoaDistanceTableBA& table, ParticleSet& P) {
    #pragma omp parallel for
    for (int iat = 0; iat < table.Ntargets; ++iat) {
        const auto& tgt = P.R[iat];
        DistanceHelper::computeDistances(tgt, table.Origin->RSoA,
                                           table.Distances[iat], table.Displacements[iat],
                                           0, table.Nsources);
    }
}

// Explicit Template Instantiations
#include <vector>
#include <array>
template struct SoaDistanceTableBA;