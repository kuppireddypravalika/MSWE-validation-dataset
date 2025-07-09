#include "graph.h"

long resolve(const ds::Graph& src, int repeat) {
    long result = 0;
    ds::Graph dest;
    dest.nodes.reserve(src.nodes.size());
    dest.callNodes.reserve(src.callNodes.size());
    dest.auxCallNodes.reserve(src.auxCallNodes.size());

    for (int i = 0; i < repeat; ++i) {
        dest.mergeInGraph(src, ds::StripAllocaBit);
        result += std::accumulate(dest.nodes.begin(), dest.nodes.end(), 0LL,
            [](long sum, const ds::Node& n) {
                return sum + std::accumulate(n.data.begin(), n.data.end(), 0);
            }
        );
    }
    return result;
}
