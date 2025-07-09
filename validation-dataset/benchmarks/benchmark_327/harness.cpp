#include <chrono>
#include <iostream>
#include <string>
#include <vector>

struct Fragment {
    bool blacklisted = false;
    void blacklist();
};

struct TreeInfo {
    int mismatchCount = 0;
};

const int MAX_MISMATCH = 4;

void execute_tree(Fragment& f, TreeInfo* ti);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int repeat = 200;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stoi(arg.substr(9));
    }

    Fragment f;
    TreeInfo ti;
    int result = 0;

    if (mode == "correct") {
        for (int i = 0; i < repeat; ++i) {
            ++result;
            if (!f.blacklisted)
                execute_tree(f, &ti);
        }
        std::cout << result << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int i = 0; i < repeat; ++i) {
            ++result;
            if (!f.blacklisted)
                execute_tree(f, &ti);
        }
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
}
