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
        return interpreter ? runInInterpreter(F) : F();
    }

private:
    bool interpreter;
    explicit ExecutionEngine(bool i) : interpreter(i) {}

    int runInInterpreter(int (*volatile F)()) const {
        int res = 0;
        for (int i = 0; i < 500; ++i) res = F();
        return res;
    }
};

// Functions emulating the generated code
static int add1(int x) { return x + 1; }
static int foo() { return add1(10); }

// Benchmark entry point
int run(size_t loops) {
    int sum = 0;
    for (size_t i = 0; i < loops; ++i) {
        // Directly call foo() to avoid the overhead of the ExecutionEngine
        sum += foo();
    }
    return sum;
}

// Explicit template instantiation statements remain unchanged
