#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include "compiler_stub.h"
Compiler::value pow_mpz_int(Compiler&, std::vector<Compiler::value>);

__mpz_struct make_mpz(long v) { return {v}; }

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    std::string impl = "original"; // ignored but parsed for compatibility
    int repeat = 300000;
    int exponent = 100;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct") mode = "correct";
        else if (arg == "--mode=perf") mode = "perf";
        else if (arg.rfind("--impl=",0)==0) impl = arg.substr(8);
        else if (arg.rfind("--repeat=",0)==0) repeat = std::stoi(arg.substr(9));
        else if (arg.rfind("--exp=",0)==0) exponent = std::stoi(arg.substr(6));
    }

    Compiler c;
    __mpz_struct a = make_mpz(3);
    Compiler::value arg0{&a};
    Compiler::value arg1{(void*)(intptr_t)exponent};
    std::vector<Compiler::value> args{arg0, arg1};

    if (mode == "correct") {
        auto r = pow_mpz_int(c, args);
        auto val = (__mpz_struct*)r.v;
        std::cout << val->v << std::endl;
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        for (int i=0;i<repeat;i++) pow_mpz_int(c, args);
        auto t2 = clock::now();
        std::cout << "Time: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count() << " ms\n";
    }
    return 0;
}
