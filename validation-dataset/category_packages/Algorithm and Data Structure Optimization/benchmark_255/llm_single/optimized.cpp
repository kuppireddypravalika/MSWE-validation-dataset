#include <vector>
#include <set>
#include <algorithm>

struct Node { int val; }; 

template <typename T>
class SetVector {
    std::vector<T> Vec;
    std::set<T> Set;
public:
    bool insert(T V) {
        auto result = Set.insert(V);
        if (result.second) { 
            Vec.push_back(V);
            return true;
        }
        return false;
    }
    bool remove(T V) {
        if (Set.erase(V) > 0) {
            auto VI = std::find(Vec.begin(), Vec.end(), V);
            if (VI != Vec.end()) {
                Vec.erase(VI);
            }
            return true;
        }
        return false;
    }
    bool empty() const { return Vec.empty(); }
    T pop_back_val() {
        T V = Vec.back();
        Vec.pop_back();
        Set.erase(V);
        return V;
    }
};

size_t run_algorithm(int iterations, int items) {
    std::vector<Node> nodes(items);
    for (int i = 0; i < items; ++i) nodes[i].val = i;
    size_t sum = 0;
    for (int it = 0; it < iterations; ++it) {
        SetVector<Node*> WorkList;
        SetVector<Node*> CombineList;
        for (auto &n : nodes) {
            WorkList.insert(&n);
            CombineList.insert(&n);
        }
        while (!WorkList.empty()) {
            Node* cur = WorkList.pop_back_val();
            CombineList.remove(cur);
            sum += cur->val;
        }
        while (!CombineList.empty()) {
            Node* cur = CombineList.pop_back_val();
            sum += cur->val;
        }
    }
    return sum;
}

// Explicit template instantiation statements
template class SetVector<Node*>;
