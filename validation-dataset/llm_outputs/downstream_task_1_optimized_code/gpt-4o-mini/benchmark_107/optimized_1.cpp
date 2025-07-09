// original.cpp
#include "sequence.hpp"

// optimized sort_sequence function

template <typename Seq>
void sort_sequence(Seq& seq) {
    // Using the unchecked wrapper for better performance.
    auto unw = unchecked(seq);
    pdqsort(unw);
}

// explicit instantiation for Sequence<int>
template void sort_sequence(Sequence<int>&);
