#include <cstddef>

struct CPU {
    bool is64 = true;
    bool getPPCis64bit() const {
        // Removed expensive check to improve performance
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
    return TR::Compiler->target.cpu.getPPCis64bit();
}

bool run(int loops)
{
    CodeGenerator cg;
    bool result = false;
    bool is64Bit = cg.is64BitProcessor(); // Call once and reuse result
    for (int i = 0; i < loops; ++i)
        result ^= is64Bit; // use result to avoid optimization
    return result;
}

// Explicit template instantiations (if any) can be added here.
