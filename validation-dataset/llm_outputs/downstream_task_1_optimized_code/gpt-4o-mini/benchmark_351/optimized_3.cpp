#include "bitvector.h"

BitVector& BitVector::operator^=(const BitVector& other) noexcept {
    const auto max_bit_size = std::max(bit_size_, other.GetSize());
    const auto min_byte_size = std::min(data_.size(), other.GetData().size());

    // Resize the current BitVector to accommodate the larger size while initializing new bits to zero
    Resize(max_bit_size, false);

    // Perform XOR for the overlapping bytes
    auto* this_data = data_.data();
    const auto* other_data = other.GetData().data();

    for (std::size_t i = 0; i < min_byte_size; ++i) {
        this_data[i] ^= other_data[i];
    }

    // Only process the remaining bytes from the other BitVector if necessary
    if (min_byte_size < other.GetData().size()) {
        for (std::size_t i = min_byte_size; i < other.GetData().size(); ++i) {
            this_data[i] = other_data[i]; // Overwrite with prior to XOR operation
        }
    }

    return *this;
}