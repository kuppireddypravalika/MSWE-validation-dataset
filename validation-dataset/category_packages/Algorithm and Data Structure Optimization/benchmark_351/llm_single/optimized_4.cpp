#include "bitvector.h"

BitVector& BitVector::operator^=(const BitVector& other) noexcept {
    const std::size_t max_bit_size = std::max(bit_size_, other.GetSize());
    const std::size_t min_byte_size = std::min(data_.size(), other.GetData().size());

    if (max_bit_size > data_.size()) {
        Resize(max_bit_size, true);
    }

    std::size_t i = 0;
    for (; i < min_byte_size; ++i) {
        data_[i] ^= other.GetData()[i];
    }

    const auto* other_data = other.GetData().data();
    for (; i < other.GetData().size(); ++i) {
        data_[i] ^= other_data[i];
    }

    return *this;
}
