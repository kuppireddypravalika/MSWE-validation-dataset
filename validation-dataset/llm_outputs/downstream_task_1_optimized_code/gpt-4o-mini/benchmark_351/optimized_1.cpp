#include "bitvector.h"

BitVector& BitVector::operator^=(const BitVector& other) noexcept {
    const std::size_t max_bit_size = std::max(bit_size_, other.GetSize());
    const std::size_t min_byte_size = std::min(data_.size(), other.GetData().size());

    Resize(max_bit_size, true);

    // Use a pointer for faster access
    const std::byte* other_data = other.GetData().data();
    std::byte* data_ptr = data_.data();

    // Optimize XOR operation with a loop unrolling technique
    std::size_t i = 0;
    for (; i + 3 < min_byte_size; i += 4) {
        data_ptr[i] ^= other_data[i];
        data_ptr[i + 1] ^= other_data[i + 1];
        data_ptr[i + 2] ^= other_data[i + 2];
        data_ptr[i + 3] ^= other_data[i + 3];
    }

    // Handle the remaining bytes
    for (; i < min_byte_size; ++i) {
        data_ptr[i] ^= other_data[i];
    }

    // If other vector has additional bytes
    if (other.GetData().size() > min_byte_size) {
        for (std::size_t j = min_byte_size; j < other.GetData().size(); ++j) {
            data_ptr[j] ^= other_data[j];
        }
    }
    return *this;
}