#include "song_filter.hpp"

// Optimized Parse consumes the buffer using shift(), avoiding index arithmetic

void SongFilter::Parse(ConstBuffer<const char*> args, bool fold_case) {
    if (args.empty())
        throw std::runtime_error("Incorrect number of filter arguments");

    do {
        if (args.size < 2)
            throw std::runtime_error("Incorrect number of filter arguments");

        const char* tag = args.shift();
        const char* value = args.shift();
        Parse(tag, value, fold_case);
    } while (!args.empty());
}

