#ifndef BITVECTOR_H
#define BITVECTOR_H

#include <algorithm>
#include <cstddef>
#include <vector>

class BitVector {
public:
    explicit BitVector(std::size_t n_bits = 0)
        : data_((n_bits + 7) / 8), bit_size_(n_bits) {}

    std::size_t GetSize() const { return bit_size_; }
    const std::vector<std::byte>& GetData() const { return data_; }
    std::vector<std::byte>& MutableData() { return data_; }

    void Resize(std::size_t n_bits, bool zero_fill) {
        if (bit_size_ == n_bits) return;
        bit_size_ = n_bits;
        std::size_t bytes = (n_bits + 7) / 8;
        if (zero_fill) {
            if (bytes > data_.size())
                data_.insert(data_.end(), bytes - data_.size(), std::byte{});
            else
                data_.resize(bytes);
        } else {
            data_.resize(bytes);
        }
        TruncateToFit();
    }

    BitVector& operator^=(const BitVector& other) noexcept;

private:
    void TruncateToFit() {
        std::size_t offset = bit_size_ % 8;
        if (offset && !data_.empty()) {
            data_.back() &= std::byte(0xFFu >> (8 - offset));
        }
    }

    std::vector<std::byte> data_;
    std::size_t bit_size_;
};

#endif
