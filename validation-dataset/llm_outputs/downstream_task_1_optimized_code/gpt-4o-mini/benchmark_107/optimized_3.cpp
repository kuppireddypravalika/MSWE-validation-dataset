#include "sequence.hpp"

// optimized sort_sequence implementation

template <typename Seq>
void sort_sequence(Seq& seq) {
    if (seq.size() > 1) {
        pdqsort(seq);
    }
}

// explicit instantiation for Sequence<int>
template void sort_sequence(Sequence<int>&);