#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

enum StorageClass { Function = 0, UniformConstant, Workgroup, Generic, Private };

struct Type {};
struct PointerType : Type {
    Type* element;
    uint32_t addrSpace;
    PointerType(Type* e, uint32_t a) : element(e), addrSpace(a) {}
    Type* getPointerElementType() const { return element; }
    uint32_t getPointerAddressSpace() const { return addrSpace; }
};

struct SPIRVType { PointerType* ptr; };
struct SPIRVVariable { StorageClass sc; SPIRVType* ty; };

uint64_t translate_variables(const std::vector<SPIRVVariable>& vars, int repeat);

std::vector<SPIRVVariable> create_vars(size_t n) {
    std::vector<SPIRVVariable> vars(n);
    static Type base;
    for (size_t i = 0; i < n; ++i) {
        auto sc = static_cast<StorageClass>(i % 5);
        uint32_t as = 0;
        switch (sc) {
        case Function: as = 0; break;
        case UniformConstant: as = 1; break;
        case Workgroup: as = 3; break;
        case Generic: as = 4; break;
        case Private: as = 5; break;
        }
        PointerType* ptr = new PointerType(&base, as);
        vars[i] = { sc, new SPIRVType{ptr} };
    }
    return vars;
}

int main(int argc, char* argv[]) {
    std::string mode = "perf";
    size_t vars = 10000;
    int repeat = 250000;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--mode=correct")
            mode = "correct";
        else if (arg == "--mode=perf")
            mode = "perf";
        else if (arg.rfind("--vars=", 0) == 0)
            vars = std::stoul(arg.substr(7));
        else if (arg.rfind("--repeat=", 0) == 0)
            repeat = std::stoi(arg.substr(9));
    }

    auto data = create_vars(vars);

    if (mode == "correct") {
        uint64_t sum = translate_variables(data, 1);
        std::cout << sum << '\n';
    } else {
        using clock = std::chrono::high_resolution_clock;
        auto t1 = clock::now();
        uint64_t sum = translate_variables(data, repeat);
        auto t2 = clock::now();
        std::cout << "Time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
    }
    return 0;
}
