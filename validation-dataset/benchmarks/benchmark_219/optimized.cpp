#include <vector>
#include <numeric>
#include <stdexcept>
#include "big_list.h"

struct network_topology {
    std::vector<int> data;
    unsigned long long sum = 0;
    static network_topology random(std::vector<int> topology) {
        network_topology n;
        n.data = std::move(topology);
        n.sum = std::accumulate(n.data.begin(), n.data.end(), 0ull);
        return n;
    }
};

class network {
public:
    network(std::initializer_list<int> initializer_list) {
        if (initializer_list.size() < 3)
            throw std::runtime_error("needs >=3 elements");
        this->topology_ = network_topology::random(std::vector<int>{initializer_list});
    }
    unsigned long long sum() const { return topology_.sum; }
private:
    network_topology topology_;
};

unsigned long long run_network() {
    network n({BIG_LIST});
    return n.sum();
}
