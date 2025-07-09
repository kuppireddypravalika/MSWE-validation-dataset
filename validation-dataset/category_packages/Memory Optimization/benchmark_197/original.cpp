#include <cstddef>
#include <vector>

namespace bench197 {

struct FakeChain {
    size_t work;
    void LoadTree() const {
        volatile size_t sink = 0;
        for (size_t i = 0; i < work; ++i) sink += i % 7;
    }
};

struct LeafList {
    size_t count;
    size_t GetEntries() const { return count; }
};

struct FakeTree {
    FakeChain chain;
    size_t entries;
    LeafList leaves;
    FakeTree(size_t e, size_t nleaves, size_t w) : chain{w}, entries(e) { leaves.count = nleaves; }
    void* GetCurrentFile() const { return nullptr; }
    LeafList* GetListOfLeaves() { return &leaves; }
    size_t GetEntries() { chain.LoadTree(); return entries; }
    size_t GetEntriesFast() const { return entries; }
};

struct FakeFilePrefetch {
    void* file;
    int buf;
    FakeFilePrefetch(void* f, int b) : file(f), buf(b) {}
};

class FakeTreeFilePrefetch : public FakeFilePrefetch {
public:
    size_t fEntryMin;
    size_t fEntryMax;
    size_t fEntryNext;
    size_t* fBranches;
    FakeTreeFilePrefetch(FakeTree* tree, int buffersize)
        : FakeFilePrefetch(tree->GetCurrentFile(), buffersize),
          fEntryMin(0),
          fEntryMax(tree->GetEntries()),
          fEntryNext(0),
          fBranches(nullptr) {
        fEntryNext = fEntryMin + 10;
        int nleaves = static_cast<int>(tree->GetListOfLeaves()->GetEntries());
        fBranches = new size_t[nleaves + 10];
    }
    ~FakeTreeFilePrefetch() { delete[] fBranches; }
    size_t entryMax() const { return fEntryMax; }
};

} // namespace bench197

size_t run(int repeat, size_t entries, size_t leaves, size_t work) {
    bench197::FakeTree tree(entries, leaves, work);
    size_t sum = 0;
    for (int i = 0; i < repeat; ++i) {
        bench197::FakeTreeFilePrefetch pf(&tree, 0);
        sum += pf.entryMax();
    }
    return sum;
}
