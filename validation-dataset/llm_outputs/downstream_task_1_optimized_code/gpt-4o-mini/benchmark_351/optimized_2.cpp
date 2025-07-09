#include "bitvector.h"

BitVector& BitVector::operator^=(const BitVector& other) noexcept {
    const auto max_bit_size = std::max(bit_size_, other.GetSize());
    const auto min_byte_size = std::min(data_.size(), other.GetData().size());

    Resize(max_bit_size, false);

    // Use std::transform to apply XOR operation in a more optimized manner.
    std::transform(data_.begin(), data_.begin() + min_byte_size, other.GetData().begin(), data_.begin(), std::bit_xor<std::byte>());

    // Handle remaining bits if the other BitVector is larger.
    if (data_.size() < other.GetData().size()) {
        std::copy(other.GetData().begin() + min_byte_size, other.GetData().end(), data_.begin() + min_byte_size);
    }
    return *this;
}