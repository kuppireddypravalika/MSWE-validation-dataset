#include <vector>
#include <cstdint>
#include <cstring>
#include <algorithm>
#include <numeric>
#include <immintrin.h>
#include "shared.h"

namespace optimized {

template <class ELFT>
void AtomSection<ELFT>::write(void* /*writer*/, std::vector<uint8_t>& buffer) {
    uint8_t* chunkBuffer = buffer.data();
    for (auto& ai : _atoms) {
        const DefinedAtom* definedAtom = ai->_atom;
        if ((definedAtom->contentType() == DefinedAtom::typeZeroFill) ||
            (definedAtom->contentType() == DefinedAtom::typeZeroFillFast))
            continue;
        const auto& content = definedAtom->rawContent();
        uint64_t contentSize = content.size();
        if (contentSize == 0)
            continue;
        uint8_t* atomContent = chunkBuffer + ai->_fileOffset;
        std::memcpy(atomContent, content.data(), contentSize);
    }
}

} // namespace optimized

static float checksum(const std::vector<uint8_t>& buf) {
    uint64_t sum = std::accumulate(buf.begin(), buf.end(), uint64_t{0});
    return static_cast<float>(sum & 0xFFFFFFFF);
}

float run(int repeat, size_t numAtoms, size_t atomSize) {
    optimized::AtomSection<dummyELF> section;
    section._atoms.reserve(numAtoms);
    std::vector<uint8_t> buffer;
    buffer.reserve(numAtoms * atomSize);
    for (size_t i = 0; i < numAtoms; ++i) {
        auto atom = new DefinedAtom(DefinedAtom::typeData, atomSize);
        uint64_t offset = buffer.size();
        buffer.resize(offset + atom->rawContent().size());
        std::copy(atom->rawContent().begin(), atom->rawContent().end(), buffer.begin() + offset);
        section._atoms.push_back(new AtomLayout{atom, offset});
    }
    float result = 0.0f;
    for (int r = 0; r < repeat; ++r) {
        section.write(nullptr, buffer);
        result += checksum(buffer);
    }
    return result;
}

// Explicit template instantiations
template class original::AtomSection<dummyELF>;
