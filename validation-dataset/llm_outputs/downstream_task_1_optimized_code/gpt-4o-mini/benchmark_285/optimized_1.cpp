#include "mock_tree.h"
#include <vector>

static MockTree* merge_trees_impl(std::vector<MockTree*>& trees, const char* options) {
    if (trees.empty()) return nullptr;
    MockTree* newtree = nullptr;
    size_t total_entries = 0;
    bool first_tree = true;

    // Precompute total entries for the new tree
    for (const auto* tree : trees) {
        size_t nentries = tree->GetEntries();
        if (nentries > 0) total_entries += nentries;
    }

    // Allocate new tree only once with total entries
    newtree = new MockTree(total_entries);
    if (!newtree) return nullptr;

    for (auto* tree : trees) {
        size_t nentries = tree->GetEntries();
        if (nentries == 0) continue;
        if (first_tree) {
            newtree->ResetBranchAddresses();
            first_tree = false;
        } else {
            newtree->CopyAddresses(tree);
        }
        newtree->CopyEntries(tree, nentries, options);
        tree->ResetBranchAddresses();
    }
    return newtree;
}

MockTree* merge_trees(std::vector<MockTree*>& trees, const char* options) {
    return merge_trees_impl(trees, options);
}

// Explicit template instantiations
