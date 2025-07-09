#include "graph.h"

long resolve(const ds::Graph& src, int repeat) {
    long result = 0;
    // Preallocate destination graph to avoid repeated allocations
    ds::Graph dest(src.nodes.size(), src.callNodes.size(), src.auxCallNodes.size());

    for (int i = 0; i < repeat; ++i) {
        // Clear destination graph data without allocation
        dest.nodes.clear();
        dest.callNodes.clear();
        dest.auxCallNodes.clear();
        
        long sum = 0;
        // Utilize move semantics and reserve to optimize merging
        dest.nodes.reserve(src.nodes.size());
        dest.callNodes.reserve(src.callNodes.size());
        dest.auxCallNodes.reserve(src.auxCallNodes.size());

        // Merge nodes with a more efficient loop
        for (const auto& n : src.nodes) {
            dest.nodes.emplace_back(n);
        }
        if (!(ds::DontCloneCallNodes & ds::StripAllocaBit)) {
            for (const auto& n : src.callNodes) {
                dest.callNodes.emplace_back(n);
            }
        }
        if (!(ds::DontCloneAuxCallNodes & ds::StripAllocaBit)) {
            for (const auto& n : src.auxCallNodes) {
                dest.auxCallNodes.emplace_back(n);
            }
        }

        // Sum data leveraging const references
        for (const auto& n : dest.nodes) {
            for (int v : n.data) {
                sum += v;
            }
        }
        result += sum;
    }
    return result;
}

// Explicit template instantiations
