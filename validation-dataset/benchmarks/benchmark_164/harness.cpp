#include <chrono>
#include <iostream>
#include <string>
#include <vector>

struct ListLog {
    std::vector<std::vector<std::string>> rows;
    size_t width = 0;
    void Append(const std::vector<std::string>& cols, int level, int autofit_col = -1) {
        rows.push_back(cols);
        if (autofit_col >= 0) {
            volatile size_t dummy = 0;
            for (int i = 0; i < 35000; ++i)
                dummy += rows.size();
            width = dummy;
        }
    }
};

size_t run(size_t lines);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    size_t lines = 50000;
    int repeat = 20;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--lines=", 0) == 0) lines = std::stoul(arg.substr(8));
        else if (arg.rfind("--repeat=", 0) == 0) repeat = std::stoi(arg.substr(9));
    }

    if (mode == "correct") {
        size_t result = run(lines);
        std::cout << result << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int r = 0; r < repeat; ++r) run(lines);
        auto t2 = clock::now();

        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
}
