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
    static uint32_t rmap(StorageClass sc) {
        static const std::unordered_map<int, uint32_t> table = {
            {Function, 0}, {UniformConstant, 1}, {Workgroup, 3}, {Generic, 4}, {Private, 5}
        };
        return table.at(sc);
    }
};

uint64_t translate_variables(const std::vector<SPIRVVariable>& vars, int repeat) {
    uint64_t sum = 0;
    for (int r = 0; r < repeat; ++r) {
        for (const auto& v : vars) {
            Type* varType = transType(v.ty)->getPointerElementType();
            (void)varType;
            uint32_t addrSpace = SPIRSPIRVAddrSpaceMap::rmap(v.sc);
            sum += addrSpace;
        }
    }
    return sum;
}
