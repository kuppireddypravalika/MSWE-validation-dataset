#include <iostream>
#include <vector>

// Minimal placeholder types mimicking LLVM classes
struct Module {
    const char* name;
    Module(const char* n) : name(n) {}
};

struct ExistingModuleProvider {
    Module* M;
    explicit ExistingModuleProvider(Module* m) : M(m) {}
};

class ExecutionEngine {
public:
    static ExecutionEngine* create(ExistingModuleProvider* /*MP*/, bool /*forceInterpreter*/) {
        return new ExecutionEngine();
    }

    int runFunction(int (*volatile F)(), const std::vector<int>& /*args*/) const {
        return F();
    }

private:
    ExecutionEngine() {}
};

// Functions emulating the generated code
static int add1(int x) { return x + 1; }
static int foo() { return add1(10); }

// Benchmark entry point
int run(size_t loops) {
    Module* M = new Module("test");
    ExistingModuleProvider* MP = new ExistingModuleProvider(M);
    ExecutionEngine* EE = ExecutionEngine::create(MP, false); // No interpreter overhead

    int sum = 0;
    for (size_t i = 0; i < loops; ++i) {
        sum += EE->runFunction(foo, {});
    }

    delete EE;
    delete MP;
    delete M;
    return sum;
}

// Replicating explicit template instantiation statements
// (none present in the original code)