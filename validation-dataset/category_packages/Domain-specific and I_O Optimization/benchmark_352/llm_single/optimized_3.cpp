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

    __attribute__((noinline)) int runFunction(int (*volatile F)(), const std::vector<int>& /*args*/) const {
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
    Module M("test");
    ExistingModuleProvider MP(&M);
    ExecutionEngine* EE = ExecutionEngine::create(&MP, false);

    int sum = 0;
    for (size_t i = 0; i < loops; ++i) {
        sum += EE->runFunction(foo, {});
    }

    delete EE;
    return sum;
}
