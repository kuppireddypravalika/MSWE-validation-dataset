#include "graph.h"

long resolve(const ds::Graph& src, int repeat) {
    long result = 0;
    for (int i = 0; i < repeat; ++i) {
        ds::Graph dest(src.nodes.size(), src.callNodes.size(), src.auxCallNodes.size());
        std::copy(src.nodes.begin(), src.nodes.end(), dest.nodes.begin());
        if (!(ds::DontCloneCallNodes & ds::StripAllocaBit)) {
            std::copy(src.callNodes.begin(), src.callNodes.end(), dest.callNodes.begin());
        }
        if (!(ds::DontCloneAuxCallNodes & ds::StripAllocaBit)) {
            std::copy(src.auxCallNodes.begin(), src.auxCallNodes.end(), dest.auxCallNodes.begin());
        }

        long sum = 0;
        for (const auto& n : dest.nodes) {
            for (int v : n.data) {
                sum += v;
            }
        }
        result += sum;
    }
    return result;
}