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

class SPIRSPIRVAddrSpaceMap {
public:
    static uint32_t rmap(StorageClass sc) {
        static const std::array<uint32_t, 6> table = {
            0, // Function
            1, // UniformConstant
            3, // Workgroup
            4, // Generic
            5  // Private
        };
        return table[sc];
    }
};

uint64_t translate_variables(const std::vector<SPIRVVariable>& vars, int repeat) {
    uint64_t sum = 0;
    uint32_t addrSpace;
    Type* varType;

    for (int r = 0; r < repeat; ++r) {
        for (const auto& v : vars) {
            varType = transType(v.ty)->getPointerElementType();
            (void)varType;
            addrSpace = SPIRSPIRVAddrSpaceMap::rmap(v.sc);
            sum += addrSpace;
        }
    }
    return sum;
}
