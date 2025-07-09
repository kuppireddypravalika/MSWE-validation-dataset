// optimized.cpp
#include "minicurl.hpp"

std::string MiniCurl::getURL(size_t size) {
    d_data.clear();
    perform_request(size);
    // Return the internal buffer directly without copying
    return std::move(d_data); // Use move semantics to avoid copying
}

std::string getURL(MiniCurl& curl, size_t size) {
    return curl.getURL(size);
}
