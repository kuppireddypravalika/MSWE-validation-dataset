#pragma once
#include <cstdint>
#include <cstdlib>
#include <algorithm>

struct Image {
    int sx = 0;
    int sy = 0;
    int pitch = 0;
    uint8_t* data = nullptr;

    Image() = default;
    Image(int w, int h) { allocate(w, h); }
    ~Image() { free(data); }
    Image(const Image&) = delete;
    Image& operator=(const Image&) = delete;
    Image(Image&& o) noexcept : sx(o.sx), sy(o.sy), pitch(o.pitch), data(o.data) { o.data=nullptr; }
    Image& operator=(Image&& o) noexcept {
        if (this != &o) {
            free(data);
            sx=o.sx; sy=o.sy; pitch=o.pitch; data=o.data; o.data=nullptr;
        }
        return *this;
    }
    void allocate(int w, int h) {
        sx=w; sy=h; pitch=w;
        size_t bytes = size_t(w)*h;
        free(data);
        if (posix_memalign((void**)&data, 32, (bytes + 31) & ~size_t(31))) data=nullptr;
    }
};

void downsample_image(const Image& src, Image& dst);
