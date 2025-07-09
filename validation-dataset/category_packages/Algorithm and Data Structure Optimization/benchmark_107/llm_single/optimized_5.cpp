#include "sequence.hpp"

// optimized sort function

template <typename Seq>
void sort_sequence(Seq& seq) {
    // Utilize the unchecked view of the sequence for performance
    auto unchecked_seq = unchecked(seq);
    pdqsort(unchecked_seq);
}

// explicit instantiation for Sequence<int>
template void sort_sequence(Sequence<int>&);
