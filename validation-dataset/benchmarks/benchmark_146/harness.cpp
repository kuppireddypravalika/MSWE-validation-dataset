#include <chrono>
#include <iostream>
#include <string>

int work(long long iterations);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    int repeat = 1;
    long long iterations = 6000000000LL; // default workload
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
        else if (arg.rfind("--iter=",0)==0) iterations = std::stoll(arg.substr(7));
    }

    if (mode == "correct") {
        int res = work(iterations);
        std::cout << res << '\n';
        return 0;
    }

    using clock = std::chrono::high_resolution_clock;
    auto t1 = clock::now();
    for(int r=0;r<repeat;++r) work(iterations);
    auto t2 = clock::now();

    std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << " ms\n";
    return 0;
}
