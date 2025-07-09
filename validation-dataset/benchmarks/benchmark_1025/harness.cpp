#include <chrono>
#include <iostream>
#include <string>
#include <omp.h>

extern "C" double run(size_t repeat);

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    size_t repeat = 1500000;

    for(int i=1;i<argc;++i){
        std::string arg = argv[i];
        if(arg=="--mode=correct") mode = "correct";
        else if(arg=="--mode=perf") mode = "perf";
        else if(arg.rfind("--repeat=",0)==0) repeat = std::stoull(arg.substr(9));
    }

    if(mode=="correct") {
        double result = run(10);
        std::cout << result << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        double dummy = run(repeat);
        auto t2 = clock::now();
        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count() << " ms\n";
        if(dummy==123456789.0) std::cerr << ""; // prevent optimization
    }
}
