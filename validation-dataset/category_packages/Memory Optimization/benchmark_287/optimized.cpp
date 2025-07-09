#include "graph.h"

long resolve(const ds::Graph& src, int repeat) {
    long result = 0;
    for (int i = 0; i < repeat; ++i) {
        ds::Graph dest;
        result += dest.mergeInGraph(src, ds::StripAllocaBit | ds::DontCloneCallNodes | ds::DontCloneAuxCallNodes);
    }
    return result;
}
