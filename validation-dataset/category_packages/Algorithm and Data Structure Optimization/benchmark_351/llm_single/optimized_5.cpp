#include "bitvector.h"

BitVector& BitVector::operator^=(const BitVector& other) noexcept {
    const std::size_t max_bit_size = std::max(bit_size_, other.GetSize());
    const std::size_t min_byte_size = std::min(data_.size(), other.GetData().size());

    Resize(max_bit_size, false);

    std::size_t i = 0;
    for (; i < min_byte_size; ++i) {
        data_[i] ^= other.GetData()[i];
    }

    // Avoiding out-of-bounds access by checking byte sizes
    if (i < other.GetData().size()) {
        std::memcpy(data_.data() + i, other.GetData().data() + i, other.GetData().size() - i);
    }

    return *this;
}
