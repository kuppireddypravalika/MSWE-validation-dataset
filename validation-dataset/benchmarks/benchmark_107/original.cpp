#include "sequence.hpp"

// original sort uses checked iterators

template <typename Seq>
void sort_sequence(Seq& seq) {
    pdqsort(seq);
}

// explicit instantiation for Sequence<int>
template void sort_sequence(Sequence<int>&);
