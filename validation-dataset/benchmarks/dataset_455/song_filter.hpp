#pragma once
#include <stdexcept>
#include <string>
#include <vector>

// Minimal ConstBuffer similar to MPD's util::ConstBuffer
// Provides size, empty(), operator[] and shift() used in the patch

template <typename T>
struct ConstBuffer {
    const T* data;
    std::size_t size;

    bool empty() const { return size == 0; }

    const T& operator[](std::size_t i) const { return data[i]; }

    const T& shift() {
        const T& value = *data;
        ++data;
        --size;
        return value;
    }
};

class SongFilter {
public:
    struct Item {
        std::string tag;
        std::string value;
        Item(const char* t = "", const char* v = "") : tag(t), value(v) {}
    };

    std::vector<Item> items;

    void Parse(const char* tag, const char* value, bool fold_case);
    void Parse(ConstBuffer<const char*> args, bool fold_case);
};

inline void SongFilter::Parse(const char* tag, const char* value, bool /*fold_case*/) {
    items.emplace_back(tag, value);
}
