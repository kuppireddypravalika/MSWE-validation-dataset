#include "mock_tree.h"
#include <vector>

static MockTree* merge_trees_impl(std::vector<MockTree*>& trees, const char* options) {
    if (trees.empty()) return nullptr;
    MockTree* newtree = nullptr;
    size_t totalEntries = 0;

    // Calculate total entries in advance
    for (const auto* tree : trees) {
        size_t nentries = tree->GetEntries();
        if (nentries > 0) {
            totalEntries += nentries;
        }
    }

    if (totalEntries == 0) return nullptr;

    for (auto* tree : trees) {
        size_t nentries = tree->GetEntries();
        if (nentries == 0) continue;
        if (!newtree) {
            newtree = tree->CloneTree(); // options ignored
            if (!newtree) continue;
            tree->ResetBranchAddresses();
            newtree->ResetBranchAddresses();
            continue;
        }
        newtree->CopyAddresses(tree);
        newtree->CopyEntries(tree, nentries, options);
        tree->ResetBranchAddresses();
    }
    return newtree;
}

MockTree* merge_trees(std::vector<MockTree*>& trees, const char* options) {
    return merge_trees_impl(trees, options);
}

// Explicit template instantiation statements
