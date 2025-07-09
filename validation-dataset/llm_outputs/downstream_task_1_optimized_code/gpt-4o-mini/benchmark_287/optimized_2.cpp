#include "graph.h"

long resolve(const ds::Graph& src, int repeat) {
    long result = 0;
    // Reserve memory for dest graph in advance based on the source graph size
    ds::Graph dest(src.nodes.size(), src.callNodes.size(), src.auxCallNodes.size());
    for (int i = 0; i < repeat; ++i) {
        // Execute the merge once per iteration instead of creating a new Graph each time
        result += dest.mergeInGraph(src, ds::StripAllocaBit);
    }
    return result;
}

// Explicit template instantiation statements
