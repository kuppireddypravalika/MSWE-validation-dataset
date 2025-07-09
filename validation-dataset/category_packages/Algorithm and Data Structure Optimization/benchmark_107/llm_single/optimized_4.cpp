// optimized.cpp
#include "sequence.hpp"

// optimized sort_sequence that avoids checked iterators

template <typename Seq>
void sort_sequence(Seq& seq) {
    auto wrapper = unchecked(seq);
    pdqsort(wrapper);
}

// explicit instantiation for Sequence<int>
template void sort_sequence(Sequence<int>&);
