#include <vector>
#include <array>
#include <cstddef>

namespace ds {

enum MergeFlags {
    StripAllocaBit      = 1 << 0,
    DontCloneCallNodes  = 1 << 1,
    DontCloneAuxCallNodes = 1 << 2
};

static constexpr std::size_t DataSize = 128;

struct Node {
    std::array<int, DataSize> data{};
    Node() {
        for (std::size_t i = 0; i < DataSize; ++i)
            data[i] = static_cast<int>(i);
    }
};

struct Graph {
    std::vector<Node> nodes;
    std::vector<Node> callNodes;
    std::vector<Node> auxCallNodes;

    Graph(std::size_t n=0, std::size_t c=0, std::size_t a=0)
        : nodes(n), callNodes(c), auxCallNodes(a) {}

    long mergeInGraph(const Graph& src, int flags) {
        nodes.clear();
        callNodes.clear();
        auxCallNodes.clear();
        nodes.reserve(src.nodes.size());
        callNodes.reserve(src.callNodes.size());
        auxCallNodes.reserve(src.auxCallNodes.size());

        for (const auto& n : src.nodes) {
            Node copy;
            for (std::size_t i = 0; i < DataSize; ++i)
                copy.data[i] = n.data[i];
            nodes.push_back(copy);
        }
        if (!(flags & DontCloneCallNodes)) {
            for (const auto& n : src.callNodes) {
                Node copy;
                for (std::size_t i = 0; i < DataSize; ++i)
                    copy.data[i] = n.data[i];
                callNodes.push_back(copy);
            }
        }
        if (!(flags & DontCloneAuxCallNodes)) {
            for (const auto& n : src.auxCallNodes) {
                Node copy;
                for (std::size_t i = 0; i < DataSize; ++i)
                    copy.data[i] = n.data[i];
                auxCallNodes.push_back(copy);
            }
        }

        long sum = 0;
        for (const auto& n : nodes)
            for (int v : n.data)
                sum += v;
        return sum;
    }
};

} // namespace ds
