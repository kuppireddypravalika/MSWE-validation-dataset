#ifndef MOCK_TREE_H
#define MOCK_TREE_H
#include <vector>
#include <cstring>

inline bool use_fast(const char* opts) {
    return opts && std::strstr(opts, "fast");
}

struct MockTree {
    std::vector<int> data;

    MockTree() = default;
    explicit MockTree(size_t n) : data(n) {
        for (size_t i = 0; i < n; ++i) data[i] = static_cast<int>(i);
    }

    size_t GetEntries() const { return data.size(); }

    MockTree* CloneTree(long long nentries = -1, const char* opts = "") const {
        size_t n = (nentries < 0 || static_cast<size_t>(nentries) > data.size()) ? data.size() : static_cast<size_t>(nentries);
        MockTree* t = new MockTree();
        t->data.reserve(n);
        if (use_fast(opts)) {
            t->data.insert(t->data.end(), data.begin(), data.begin() + n);
        } else {
            for (size_t i = 0; i < n; ++i) {
                volatile int v = data[i]; for (int k = 0; k < 500; ++k) v += k;
                t->data.push_back((int)v);
            }
        }
        return t;
    }

    void CopyEntries(const MockTree* src, long long nentries = -1, const char* opts = "") {
        size_t n = (nentries < 0 || static_cast<size_t>(nentries) > src->data.size()) ? src->data.size() : static_cast<size_t>(nentries);
        if (use_fast(opts)) {
            data.insert(data.end(), src->data.begin(), src->data.begin() + n);
        } else {
            for (size_t i = 0; i < n; ++i) {
                volatile int v = src->data[i]; for (int k = 0; k < 500; ++k) v += k;
                data.push_back((int)v);
            }
        }
    }

    void CopyAddresses(const MockTree*) {}
    void ResetBranchAddresses() {}
};

#endif
