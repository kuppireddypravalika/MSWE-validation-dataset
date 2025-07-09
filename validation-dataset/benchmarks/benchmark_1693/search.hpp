#pragma once
#include <array>
#include <vector>
#include <memory>
#include <cmath>

struct SphericParticle {
    std::array<double,3> coord;
    double radius;
    int workload;
};

using ParticlePtr = std::shared_ptr<SphericParticle>;
using ParticleVector = std::vector<ParticlePtr>;

// Search implementation defined in original.cpp / optimized.cpp
size_t dem_fem_search(const ParticleVector& elements);
