#include <chrono>
#include <iostream>
#include <string>
#include <cstddef>

struct CPU {
    bool is64 = true;
    bool getPPCis64bit() const {
        volatile std::size_t sum = 0;
        for (int i = 0; i < 10000; ++i) sum += i;
        return is64;
    }
};

struct Target {
    bool build64 = true;
    CPU cpu;
    bool is64Bit() const { return build64; }
};

struct CompilerGlobals { Target target; };
namespace TR { CompilerGlobals* Compiler; }

class CodeGenerator {
public:
    bool is64BitProcessor();
};

bool run(int loops);

int main(int argc, char* argv[])
{
    std::string mode = "perf";
    int loops = 600000; // workload for perf mode
    int repeat = 10;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--loops=",0)==0) loops = std::stoi(arg.substr(8));
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
    }

    CompilerGlobals globals; // initialize compiler target
    globals.target.build64 = true;
    globals.target.cpu.is64 = true;
    TR::Compiler = &globals;

    if (mode == "correct") {
        int test_loops = 100; // smaller workload
        bool result = run(test_loops);
        std::cout << result << std::endl;
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int i = 0; i < repeat; ++i)
            run(loops);
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()
                  << " ms\n";
    }
    return 0;
}
