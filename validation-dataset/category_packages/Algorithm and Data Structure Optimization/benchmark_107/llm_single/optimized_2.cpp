#include "sequence.hpp"

// optimized sort_sequence using unchecked wrapper for improved performance

template <typename Seq>
void sort_sequence(Seq& seq) {
    auto unw_seq = unchecked(seq);
    pdqsort(unw_seq);
}

// explicit instantiation for Sequence<int>
template void sort_sequence(Sequence<int>&);
