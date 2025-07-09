#ifndef BYTEARRAY_H
#define BYTEARRAY_H
#include <cstring>
class ByteArray {
    char* data_;
    size_t size_;
    size_t capacity_;
public:
    ByteArray() : data_(nullptr), size_(0), capacity_(0) {}
    ByteArray(const char* buf, size_t len) : data_(new char[len]), size_(len), capacity_(len) {
        std::memcpy(data_, buf, len);
    }
    ByteArray(const ByteArray& other) : data_(new char[other.size_]), size_(other.size_), capacity_(other.size_) {
        std::memcpy(data_, other.data_, other.size_);
    }
    ByteArray& operator=(const ByteArray& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            capacity_ = other.size_;
            data_ = new char[size_];
            std::memcpy(data_, other.data_, size_);
        }
        return *this;
    }
    void assign(const char* buf, size_t len) {
        if (capacity_ < len) {
            delete[] data_;
            data_ = new char[len];
            capacity_ = len;
        }
        std::memcpy(data_, buf, len);
        size_ = len;
    }
    ~ByteArray() { delete[] data_; }
    int checksum() const {
        int sum = 0;
        for (size_t i = 0; i < size_; ++i) sum += static_cast<unsigned char>(data_[i]);
        return sum;
    }
};
#endif // BYTEARRAY_H
