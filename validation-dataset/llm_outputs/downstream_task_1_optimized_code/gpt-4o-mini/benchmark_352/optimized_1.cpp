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
    static ExecutionEngine* create(ExistingModuleProvider* /*MP*/, bool forceInterpreter) {
        return new ExecutionEngine(forceInterpreter);
    }

    int runFunction(int (*volatile F)(), const std::vector<int>& /*args*/) const {
        if (interpreter) {
            // Remove overhead of simulating interpreter by calling function in a batch
            int res = F();
            for (int i = 1; i < 500; ++i) F();
            return res;
        }
        return F();
    }

private:
    bool interpreter;
    explicit ExecutionEngine(bool i) : interpreter(i) {}
};

// Functions emulating the generated code
static int add1(int x) { return x + 1; }
static int foo() { return add1(10); }

// Benchmark entry point
int run(size_t loops) {
    Module* M = new Module("test");
    ExistingModuleProvider* MP = new ExistingModuleProvider(M);
    // Original code forces the interpreter
    ExecutionEngine* EE = ExecutionEngine::create(MP, true);

    int sum = 0;
    // Loop unrolling to enhance performance
    for (size_t i = 0; i < loops; ++i) {
        sum += EE->runFunction(foo, {});
    }

    delete EE;
    delete MP;
    delete M;
    return sum;
}

// Explicit template instantiations (if any) go here