#include "bitvector.h"

BitVector& BitVector::operator^=(const BitVector& other) noexcept {
    const auto max_bit_size = std::max(bit_size_, other.GetSize());
    const auto min_byte_size = std::min(data_.size(), other.GetData().size());

    Resize(max_bit_size, true);

    for (std::size_t i = 0; i < min_byte_size; ++i) {
        data_[i] ^= other.GetData()[i];
    }

    if (data_.size() < other.GetData().size()) {
        for (std::size_t i = min_byte_size; i < other.GetData().size(); ++i) {
            data_[i] ^= other.GetData()[i];
        }
    }
    return *this;
}
