#include "mock_tree.h"
#include <vector>

static MockTree* merge_trees_impl(std::vector<MockTree*>& trees, const char* options) {
    if (trees.empty()) return nullptr;
    MockTree* newtree = nullptr;
    size_t total_entries = 0;

    // Calculate total entries and create newtree with the first tree copied properly
    for (auto* tree : trees) {
        total_entries += tree->GetEntries();
    }

    // Ensure we have at least one tree to copy from
    if (total_entries == 0) return nullptr;
    newtree = trees[0]->CloneTree(total_entries, options);
    trees[0]->ResetBranchAddresses();
    newtree->ResetBranchAddresses();

    // Efficiently merge the other trees into newtree
    for (size_t i = 1; i < trees.size(); ++i) {
        auto* tree = trees[i];
        size_t nentries = tree->GetEntries();
        if (nentries == 0) continue;
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
