// original.cpp
#include "song_filter.hpp"

// Optimized Parse implementation that minimizes function calls and memory allocations

void SongFilter::Parse(ConstBuffer<const char*> args, bool fold_case) {
    if (args.size == 0 || args.size % 2 != 0)
        throw std::runtime_error("Incorrect number of filter arguments");

    // Reserve space in advance to avoid reallocations, reducing overhead
    items.reserve(args.size / 2);

    // Use a pointer for iterative access to reduce indexing overhead
    const char** data_ptr = const_cast<const char**>(args.data);
    for (std::size_t i = 0; i < args.size; i += 2) {
        // Directly construct Item in-place
        items.emplace_back(data_ptr[i], data_ptr[i + 1]);
    }
}

// Explicit template instantiations

template struct ConstBuffer<const char*>;