#include "mock_tree.h"
#include <vector>

static MockTree* merge_trees_impl(std::vector<MockTree*>& trees, const char* options) {
    if (trees.empty()) return nullptr;
    MockTree* newtree = nullptr;
    for (auto* tree : trees) {
        size_t nentries = tree->GetEntries();
        if (nentries == 0) continue;
        if (!newtree) {
            newtree = tree->CloneTree(-1, options); // pass options
            if (!newtree) continue;
            tree->ResetBranchAddresses();
            newtree->ResetBranchAddresses();
            continue;
        }
        newtree->CopyAddresses(tree);
        newtree->CopyEntries(tree, -1, options);
        tree->ResetBranchAddresses();
    }
    return newtree;
}

MockTree* merge_trees(std::vector<MockTree*>& trees, const char* options) {
    return merge_trees_impl(trees, options);
}

