// distance_table.hpp
#pragma once
#include <vector>
#include <array>
#include <cmath>
#include <algorithm>
#include <omp.h>

struct ParticleSet {
    std::vector<std::array<double,3>> RSoA; // source positions
    std::vector<std::array<double,3>> R;    // target positions
};

inline int getAlignment() { return 64 / sizeof(double); }

inline void FairDivideAligned(int total, int align,
                              int num_threads, int thread,
                              int& first, int& last)
{
    int chunk = (total + num_threads - 1) / num_threads;
    chunk = ((chunk + align - 1) / align) * align;
    first = thread * chunk;
    last = std::min(total, first + chunk);
    if (first > last) first = last = total;
}

struct SoaDistanceTableBA {
    int Nsources;
    int Ntargets;
    ParticleSet* Origin;
    std::vector<std::vector<double>> Distances;
    std::vector<std::vector<std::array<double,3>>> Displacements;

    SoaDistanceTableBA(ParticleSet& src, ParticleSet& tgt)
        : Nsources(src.RSoA.size()), Ntargets(tgt.R.size()), Origin(&src)
    {
        Distances.assign(Ntargets, std::vector<double>(Nsources));
        Displacements.assign(Ntargets, std::vector<std::array<double,3>>(Nsources));
    }
};

void evaluate(SoaDistanceTableBA& table, ParticleSet& P);


// original.cpp
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