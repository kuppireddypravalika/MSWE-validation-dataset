// optimized.cpp
#include "minicurl.hpp"

// Optimized implementation minimizes copying of the internal buffer
std::string MiniCurl::getURL(size_t size) {
    d_data.clear();
    d_data.reserve(size);
    perform_request(size);
    return std::move(d_data); // move instead of copy
}

std::string getURL(MiniCurl& curl, size_t size) {
    return curl.getURL(size);
}
