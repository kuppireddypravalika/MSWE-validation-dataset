#pragma once
#include <array>
#include <vector>
#include <random>

// Simple enums modeling different jobs and movement directions
enum class Job { JobA, JobB, JobC, JobD, JobE, Count };

enum class Direction { North, East, South, West };

constexpr unsigned GO_OUT_PHASES = 10;
constexpr std::size_t NUM_JOBS = static_cast<std::size_t>(Job::Count);

// Helper random function similar to RANDOM_RAND from the game
inline unsigned random_rand(std::mt19937& rng, unsigned max) {
    std::uniform_int_distribution<unsigned> dist(0, max - 1);
    // simulate heavier random generation work
    unsigned val = 0;
    for (int i = 0; i < 10; ++i) val += dist(rng);
    return val % max;
}

struct BurnedWarehouse {
    unsigned go_out_phase = 0;
    std::array<unsigned, NUM_JOBS> people{};
    std::mt19937 rng{123};

    explicit BurnedWarehouse(const std::array<unsigned, NUM_JOBS>& ppl)
        : people(ppl) {}
};

int handle_event(BurnedWarehouse& bw);
