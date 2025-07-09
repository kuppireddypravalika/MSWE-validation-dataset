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

struct DistanceHelper {
    static void computeDistances(const std::array<double,3>& tgt,
                                 const std::vector<std::array<double,3>>& src,
                                 std::vector<double>& dist,
                                 std::vector<std::array<double,3>>& disp,
                                 int first, int last)
    {
        for (int j = first; j < last; ++j) {
            std::array<double,3> diff = {src[j][0] - tgt[0],
                                         src[j][1] - tgt[1],
                                         src[j][2] - tgt[2]};
            disp[j] = diff;
            dist[j] = std::sqrt(diff[0]*diff[0] + diff[1]*diff[1] + diff[2]*diff[2]);
        }
    }
};

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
