#include "distance_table.hpp"

void evaluate(SoaDistanceTableBA& table, ParticleSet& P)
{
    #pragma omp parallel
    {
        int first, last;
        FairDivideAligned(table.Nsources, getAlignment(),
                          omp_get_num_threads(), omp_get_thread_num(),
                          first, last);
        for (int iat = 0; iat < table.Ntargets; ++iat)
            DistanceHelper::computeDistances(P.R[iat], table.Origin->RSoA,
                                              table.Distances[iat], table.Displacements[iat],
                                              first, last);
    }
}
