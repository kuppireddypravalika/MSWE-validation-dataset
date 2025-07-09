#include <cstdint>
#include <cstddef>

struct CPU {
    bool power{false};
    bool x86{false};
    bool z{true};
    bool isPower() const { return power; }
    bool isX86() const { return x86; }
    bool isZ() const { return z; }
};

struct Target { CPU cpu; };
struct Compilation { Target target; };
static Compilation g_comp; // global compilation instance

enum RecognizedMethod {
    None,
    Unsafe_compareAndExchangeInt,
    Unsafe_compareAndExchangeLong,
    Unsafe_compareAndExchangeObject,
    Unsafe_compareAndExchangeReference
};

class Symbol {
public:
    Symbol(bool nat, RecognizedMethod m) : native(nat), method(m) {}
    bool isNative() const { return native; }
    RecognizedMethod getRecognizedMethod() const { return method; }
private:
    bool native;
    RecognizedMethod method;
};

class Node {
public:
    explicit Node(Symbol* s) : symbol(s) {}
    Symbol* getSymbol() const { return symbol; }
private:
    Symbol* symbol;
};

// Original version: only checks Power and X86
bool callDoesAnImplicitAsyncCheck(Node* callNode) {
    Symbol* symbol = callNode->getSymbol();
    if (symbol->isNative() &&
        (g_comp.target.cpu.isPower() || g_comp.target.cpu.isX86())) {
        switch(symbol->getRecognizedMethod()) {
            case Unsafe_compareAndExchangeInt:
            case Unsafe_compareAndExchangeLong:
            case Unsafe_compareAndExchangeObject:
            case Unsafe_compareAndExchangeReference:
                return false;
            default:
                break;
        }
    }
    return true;
}

volatile int sink = 0;
static inline void heavyWorkload() {
    for (int i = 0; i < 1000; ++i) sink += i;
}

uint64_t run(size_t iterations) {
    Symbol sym(true, Unsafe_compareAndExchangeInt);
    Node node(&sym);
    uint64_t sum = iterations; // Initialize with iterations count for efficient summation
    for (size_t i = 0; i < iterations; ++i) {
        if (callDoesAnImplicitAsyncCheck(&node)) {
            heavyWorkload();
            --sum; // Decrement sum only if workload is done
        }
    }
    return sum;
}

// Explicit template instantiation statements
