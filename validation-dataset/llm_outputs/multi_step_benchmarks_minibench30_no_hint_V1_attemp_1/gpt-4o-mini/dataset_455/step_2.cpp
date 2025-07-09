// original.cpp
#include "song_filter.hpp"

// Optimized Parse implementation that minimizes function calls and memory allocations

void SongFilter::Parse(ConstBuffer<const char*> args, bool fold_case) {
    if (args.size == 0 || args.size % 2 != 0)
        throw std::runtime_error("Incorrect number of filter arguments");

    items.reserve(args.size / 2); // Reserve space in advance to avoid reallocations

    for (unsigned i = 0; i < args.size; i += 2) {
        items.emplace_back(args[i], args[i + 1]); // Construct in-place to avoid extra copies
    }
}

// Explicit template instantiations

template struct ConstBuffer<const char*>;