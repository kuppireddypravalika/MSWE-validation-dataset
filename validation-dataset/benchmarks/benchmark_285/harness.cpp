#include "mock_tree.h"
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

MockTree* merge_trees(std::vector<MockTree*>& trees, const char* options);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    size_t entries = 50000; // entries per tree
    size_t num_trees = 3;
    int repeat = 1200;
    std::string options = "fast";

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--entries=",0)==0) entries = std::stoul(arg.substr(10));
        else if (arg.rfind("--trees=",0)==0) num_trees = std::stoul(arg.substr(8));
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
        else if (arg.rfind("--options=",0)==0) options = arg.substr(10);
    }

    std::vector<MockTree*> trees;
    trees.reserve(num_trees);
    for (size_t i = 0; i < num_trees; ++i) trees.push_back(new MockTree(entries));

    if (mode == "correct") {
        MockTree* res = merge_trees(trees, options.c_str());
        std::cout << res->GetEntries() << std::endl;
        delete res;
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int i = 0; i < repeat; ++i) {
            MockTree* r = merge_trees(trees, options.c_str());
            delete r;
        }
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }

    for (auto* t : trees) delete t;
    return 0;
}
