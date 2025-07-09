#pragma once
#include <string>
#include <algorithm>

class MiniCurl {
public:
    std::string d_data;

    static size_t write_callback(const char* ptr, size_t size, size_t nmemb, void* userdata) {
        MiniCurl* self = static_cast<MiniCurl*>(userdata);
        self->d_data.append(ptr, size * nmemb);
        return size * nmemb;
    }

    void perform_request(size_t size) {
        d_data.clear();
        d_data.reserve(size);
        static const size_t chunk = 4096;
        std::string buf(chunk, 'x');
        size_t remaining = size;
        while (remaining) {
            size_t this_chunk = std::min(chunk, remaining);
            write_callback(buf.data(), 1, this_chunk, this);
            remaining -= this_chunk;
        }
    }

    std::string getURL(size_t size); // defined in *.cpp
};

std::string getURL(MiniCurl& curl, size_t size); // wrapper for harness
