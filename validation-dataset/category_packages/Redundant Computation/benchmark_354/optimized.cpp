#include <cstddef>

struct CPU {
    bool is64 = true;
    bool getPPCis64bit() const {
        volatile std::size_t sum = 0;
        for (int i = 0; i < 10000; ++i) sum += i; // simulate expensive check
        return is64;
    }
};

struct Target {
    bool build64 = true;
    CPU cpu;
    bool is64Bit() const { return build64; }
};

struct CompilerGlobals { Target target; };
namespace TR { extern CompilerGlobals* Compiler; }

class CodeGenerator {
public:
    bool is64BitProcessor();
};

bool CodeGenerator::is64BitProcessor()
{
    // optimized: skip expensive check when build is already 64-bit
    if (TR::Compiler->target.is64Bit())
        return true;
    else
        return TR::Compiler->target.cpu.getPPCis64bit();
}

bool run(int loops)
{
    CodeGenerator cg;
    bool result = false;
    for (int i = 0; i < loops; ++i)
        result ^= cg.is64BitProcessor();
    return result;
}
