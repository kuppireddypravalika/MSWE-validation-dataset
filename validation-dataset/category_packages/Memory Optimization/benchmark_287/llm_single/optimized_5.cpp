#include "graph.h"

long resolve(const ds::Graph& src, int repeat) {
    long result = 0;
    ds::Graph dest(src.nodes.size(), src.callNodes.size(), src.auxCallNodes.size());

    for (int i = 0; i < repeat; ++i) {
        result += dest.mergeInGraph(src, ds::StripAllocaBit);
    }
    return result;
}
