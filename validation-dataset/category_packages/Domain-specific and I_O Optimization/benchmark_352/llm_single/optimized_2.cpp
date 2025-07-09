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
    static ExecutionEngine* create(ExistingModuleProvider* /*MP*/, bool) {
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
    // Directly use execution engine without module overhead
    ExecutionEngine* EE = ExecutionEngine::create(nullptr, false);

    int sum = 0;
    for (size_t i = 0; i < loops; ++i) {
        sum += EE->runFunction(foo, {});
    }

    delete EE;
    return sum;
}

// Explicit template instantiation statements (if any) go here
