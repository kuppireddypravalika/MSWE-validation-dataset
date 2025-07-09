#include "bitvector.h"

BitVector& BitVector::operator^=(const BitVector& other) noexcept {
    auto min_byte_size = std::min(data_.size(), other.data_.size());

    std::transform(data_.cbegin(), data_.cbegin() + min_byte_size,
                   other.data_.cbegin(), data_.begin(),
                   [](std::byte a, std::byte b) { return a ^ b; });
    return *this;
}
