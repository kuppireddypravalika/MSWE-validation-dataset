#include <vector>
#include <memory>
#include <sstream>
#include <cassert>
#include <cstddef>

namespace bench181 {

struct Constant {
    virtual ~Constant() = default;
    virtual std::size_t getSize() const = 0;
};

struct ConstantInt : Constant {
    int value;
    explicit ConstantInt(int v) : value(v) {}
    std::size_t getSize() const override { return sizeof(int); }
};

struct ConstantByte : Constant {
    unsigned char value;
    explicit ConstantByte(unsigned char v) : value(v) {}
    std::size_t getSize() const override { return sizeof(unsigned char); }
};

struct StructLayout {
    std::vector<std::size_t> offsets;
    std::size_t StructSize;
};

struct ConstantStruct : Constant {
    std::vector<std::unique_ptr<Constant>> fields;
    StructLayout layout;
    std::size_t getSize() const override { return layout.StructSize; }
};

struct Printer {
    std::ostream &O;
    explicit Printer(std::ostream &out) : O(out) {}
    void printValue(const Constant *C);
};

inline void Printer::printValue(const Constant *CV) {
    if (auto CS = dynamic_cast<const ConstantStruct *>(CV)) {
        const auto &layout = CS->layout;
        const auto &vals = CS->fields;
        std::size_t sizeSoFar = 0;
        for (std::size_t i = 0, N = vals.size(); i < N; ++i) {
            const Constant *field = vals[i].get();
            std::size_t fieldSize = field->getSize();
            std::size_t padSize = ((i == N - 1 ? layout.StructSize : layout.offsets[i + 1]) -
                                   layout.offsets[i]) -
                                  fieldSize;
            sizeSoFar += fieldSize + padSize;
            printValue(field);
            // Insert the field padding unless it's zero bytes...
            O << "\t.zero\t " << padSize << "\n";
        }
        assert(sizeSoFar == layout.StructSize);
    } else if (auto CI = dynamic_cast<const ConstantInt *>(CV)) {
        O << CI->value << ' ';
    } else if (auto CB = dynamic_cast<const ConstantByte *>(CV)) {
        O << static_cast<int>(CB->value) << ' ';
    }
}

inline std::unique_ptr<ConstantStruct> createStruct() {
    auto cs = std::make_unique<ConstantStruct>();
    cs->fields.emplace_back(new ConstantInt(10));
    cs->fields.emplace_back(new ConstantInt(20));
    cs->fields.emplace_back(new ConstantByte(5));
    cs->layout.offsets = {0, 4, 8};
    cs->layout.StructSize = 12; // 3 bytes padding at end
    return cs;
}

inline std::size_t sumConstants(const Constant *C) {
    if (auto CI = dynamic_cast<const ConstantInt *>(C))
        return CI->value;
    if (auto CB = dynamic_cast<const ConstantByte *>(C))
        return CB->value;
    if (auto CS = dynamic_cast<const ConstantStruct *>(C)) {
        std::size_t s = 0;
        for (const auto &f : CS->fields)
            s += sumConstants(f.get());
        return s;
    }
    return 0;
}

std::size_t compute_impl() {
    auto cs = createStruct();
    std::ostringstream oss;
    Printer p(oss);
    p.printValue(cs.get());
    return sumConstants(cs.get());
}

} // namespace bench181

using namespace bench181;

std::size_t compute() { return compute_impl(); }

