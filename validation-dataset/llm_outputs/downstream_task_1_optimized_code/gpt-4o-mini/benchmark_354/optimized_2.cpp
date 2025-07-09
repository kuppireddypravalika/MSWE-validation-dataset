#include <cstddef>

struct CPU {
    bool is64 = true;
    bool getPPCis64bit() const {
        // Instead of simulating a workload, directly return the value
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
    // Pre-fetching the result of is64BitProcessor to avoid repeated calls
    bool is64Bit = cg.is64BitProcessor();
    for (int i = 0; i < loops; ++i)
        result ^= is64Bit; // use cached result to avoid redundant computation
    return result;
}

// Explicit template instantiation statements (assuming they are required)
// template instantiation statements go here, if needed