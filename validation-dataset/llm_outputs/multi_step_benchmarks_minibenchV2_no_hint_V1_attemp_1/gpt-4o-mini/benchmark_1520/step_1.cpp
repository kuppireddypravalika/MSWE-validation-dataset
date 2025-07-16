#include "distance_table.hpp"

void evaluate(SoaDistanceTableBA& table, ParticleSet& P) {
    #pragma omp parallel for
    for (int iat = 0; iat < table.Ntargets; ++iat) {
        DistanceHelper::computeDistances(P.R[iat], table.Origin->RSoA,
                                          table.Distances[iat], table.Displacements[iat],
                                          0, table.Nsources);
    }
}