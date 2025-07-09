#pragma once
#include <vector>
#include <cstdint>

struct DefinedAtom {
    enum ContentType { typeZeroFill, typeZeroFillFast, typeData };
    ContentType type;
    std::vector<uint8_t> content;
    DefinedAtom(ContentType t, size_t size) : type(t), content(size, static_cast<uint8_t>(size & 0xFF)) {}
    ContentType contentType() const { return type; }
    const std::vector<uint8_t>& rawContent() const { return content; }
};

struct AtomLayout {
    const DefinedAtom* _atom;
    uint64_t _fileOffset;
};

struct dummyELF {};

namespace original {
template <class ELFT>
class AtomSection {
public:
    std::vector<AtomLayout*> _atoms;
    void write(void* writer, std::vector<uint8_t>& buffer);
};
}

namespace optimized {
template <class ELFT>
class AtomSection {
public:
    std::vector<AtomLayout*> _atoms;
    void write(void* writer, std::vector<uint8_t>& buffer);
};
}
