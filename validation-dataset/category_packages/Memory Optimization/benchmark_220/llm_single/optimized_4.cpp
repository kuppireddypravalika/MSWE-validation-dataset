#include <vector>
#include <cstdint>
#include <cstring>
#include <algorithm>
#include <numeric>
#include <iostream>
#include "shared.h"

namespace optimized {

template <class ELFT>
void AtomSection<ELFT>::write(void* /*writer*/, std::vector<uint8_t>& buffer) {
    uint8_t* chunkBuffer = buffer.data();
    for (const auto& ai : _atoms) {
        const DefinedAtom* definedAtom = ai->_atom;
        if (definedAtom->contentType() == DefinedAtom::typeZeroFill ||
            definedAtom->contentType() == DefinedAtom::typeZeroFillFast)
            continue;
        const auto& content = definedAtom->rawContent();
        const uint64_t contentSize = content.size();
        if (contentSize == 0)
            continue;
        std::memcpy(chunkBuffer + ai->_fileOffset, content.data(), contentSize);
    }
}

} // namespace optimized

static float checksum(const std::vector<uint8_t>& buf) {
    uint64_t sum = std::accumulate(buf.begin(), buf.end(), uint64_t{0});
    return static_cast<float>(sum & 0xFFFFFFFF);
}

float run(int repeat, size_t numAtoms, size_t atomSize) {
    optimized::AtomSection<dummyELF> section;
    std::vector<DefinedAtom> atoms;
    atoms.reserve(numAtoms);
    for (size_t i = 0; i < numAtoms; ++i) atoms.emplace_back(DefinedAtom::typeData, atomSize);
    uint64_t offset = 0; section._atoms.reserve(numAtoms);
    for (auto& a : atoms) {
        auto* layout = new AtomLayout{&a, offset};
        section._atoms.push_back(layout);
        offset += a.rawContent().size();
    }
    std::vector<uint8_t> buffer(offset);
    float result = 0.0f;
    for (int r = 0; r < repeat; ++r) {
        section.write(nullptr, buffer);
        result += checksum(buffer);
    }
    return result;
}


// Explicit template instantiation
template class original::AtomSection<dummyELF>;
