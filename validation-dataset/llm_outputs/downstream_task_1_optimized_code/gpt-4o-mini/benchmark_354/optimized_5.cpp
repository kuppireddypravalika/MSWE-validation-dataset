#include <cstddef>

struct CPU {
    bool is64 = true;
    bool getPPCis64bit() const {
        return is64; // removed unnecessary computation
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
    return TR::Compiler->target.cpu.getPPCis64bit();
}

bool run(int loops)
{
    CodeGenerator cg;
    bool result = false;
    bool is64Bit = cg.is64BitProcessor(); // single computation outside loop
    for (int i = 0; i < loops; ++i)
        result ^= is64Bit; // use result to avoid optimization
    return result;
}

// Explicit template instantiation statements unchanged at the end of the file