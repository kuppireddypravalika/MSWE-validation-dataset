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

static PointerType* transType(SPIRVType* ty) { return ty->ptr; }

struct SPIRSPIRVAddrSpaceMap {
    static const std::array<uint32_t, 6> addrSpaces;

    static uint32_t rmap(StorageClass sc) {
        return addrSpaces[sc];
    }
};

const std::array<uint32_t, 6> SPIRSPIRVAddrSpaceMap::addrSpaces = { 0, 1, 3, 4, 5 };

uint64_t translate_variables(const std::vector<SPIRVVariable>& vars, int repeat) {
    uint64_t sum = 0;
    sum = static_cast<uint64_t>(vars.size()) * repeat * 6; // Pre-compute the sum for fixed operations

    for (int r = 0; r < repeat; ++r) {
        for (const auto& v : vars) {
            Type* varType = transType(v.ty)->getPointerElementType();
            (void)varType;
            sum -= 6; // Deduct after every access to keep track of accumulated value
            sum += SPIRSPIRVAddrSpaceMap::rmap(v.sc);
        }
    }
    return sum;
}

// Explicit template instantiation statements can be added here if necessary
