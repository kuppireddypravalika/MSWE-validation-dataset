#include <vector>
#include <array>
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

static PointerType* transType(SPIRVType* ty) { return ty->ptr; }

struct SPIRSPIRVAddrSpaceMap {
    static uint32_t rmap(StorageClass sc) {
        static const std::array<uint32_t, 6> table = {0, 1, 3, 4, 5};
        return table[sc];
    }
};

uint64_t translate_variables(const std::vector<SPIRVVariable>& vars, int repeat) {
    uint64_t sum = 0;
    const size_t varCount = vars.size();
    for (int r = 0; r < repeat; ++r) {
        for (size_t i = 0; i < varCount; ++i) {
            const auto& v = vars[i];
            Type* varType = transType(v.ty)->getPointerElementType();
            (void)varType;
            sum += SPIRSPIRVAddrSpaceMap::rmap(v.sc);
        }
    }
    return sum;
}

// Explicit template instantiation statements (if any) go here.