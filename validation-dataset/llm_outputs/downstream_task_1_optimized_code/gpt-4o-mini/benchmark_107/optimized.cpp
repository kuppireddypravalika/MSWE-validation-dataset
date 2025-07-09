#include "sequence.hpp"

// optimized sort_sequence using unchecked iterators

template <typename Seq>
void sort_sequence(Seq& seq) {
    auto unwrapped_seq = unchecked(seq);
    pdqsort(unwrapped_seq);
}

// explicit instantiation for Sequence<int>
template void sort_sequence(Sequence<int>&);