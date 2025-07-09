#include <iostream>
#include <vector>

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
        if (interpreter) {
            int res = 0;
            for (int i = 0; i < 500; ++i) res = F();
            return res;
        }
        return F();
    }

private:
    bool interpreter;
    explicit ExecutionEngine(bool i) : interpreter(i) {}
};

static int add1(int x) { return x + 1; }
static int foo() { return add1(10); }

int run(size_t loops) {
    Module* M = new Module("test");
    ExistingModuleProvider* MP = new ExistingModuleProvider(M);
    // Optimized code uses the JIT (no interpreter)
    ExecutionEngine* EE = ExecutionEngine::create(MP, false);

    std::vector<int> noargs;
    int sum = 0;
    for (size_t i = 0; i < loops; ++i) {
        sum += EE->runFunction(foo, noargs);
    }

    delete EE;
    delete MP;
    delete M;
    return sum;
}
