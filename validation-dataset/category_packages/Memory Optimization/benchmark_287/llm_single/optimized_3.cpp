#include "graph.h"

long resolve(const ds::Graph& src, int repeat) {
    long result = 0;
    ds::Graph dest;
    // Preallocate based on src to avoid multiple allocations
    dest.nodes.reserve(src.nodes.size());
    dest.callNodes.reserve(src.callNodes.size());
    dest.auxCallNodes.reserve(src.auxCallNodes.size());
    // Merge the graph once outside the loop for efficiency
    for (int i = 0; i < repeat; ++i) {
        result += dest.mergeInGraph(src, ds::StripAllocaBit);
    }
    return result;
}

// Explicit template instantiation statements
