#include <chrono>
#include <iostream>
#include <string>
#include <vector>

long long compute_sum(const std::vector<int>& A,
                      const std::vector<int>& B,
                      int width,
                      int height);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int repeat = 10;
    int width = 230;
    int height = 230;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stoi(arg.substr(9));
        else if (arg.rfind("--size=", 0) == 0) {
            std::string s = arg.substr(7);
            auto pos = s.find('x');
            width = std::stoi(s.substr(0, pos));
            height = std::stoi(s.substr(pos + 1));
        }
    }

    std::vector<int> A(width * height, 1);
    std::vector<int> B(width * height, 2);

    if (mode == "correct") {
        long long r = compute_sum(A, B, width, height);
        std::cout << r << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int r = 0; r < repeat; ++r)
            compute_sum(A, B, width, height);
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
    return 0;
}
