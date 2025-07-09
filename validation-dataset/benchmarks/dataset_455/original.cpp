#include "song_filter.hpp"

// Original Parse implementation iterates the arguments by index

void SongFilter::Parse(ConstBuffer<const char*> args, bool fold_case) {
    if (args.size == 0 || args.size % 2 != 0)
        throw std::runtime_error("Incorrect number of filter arguments");

    for (unsigned i = 0; i < args.size; i += 2)
        Parse(args[i], args[i + 1], fold_case);
}

