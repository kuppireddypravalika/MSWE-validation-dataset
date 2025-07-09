#include "sequence.hpp"

// optimized sort disables bounds checking via unchecked wrapper

template <typename Seq>
void sort_sequence(Seq& seq) {
    auto wrapper = unchecked(seq);
    pdqsort(wrapper);
}

// explicit instantiation for Sequence<int>
template void sort_sequence(Sequence<int>&);
