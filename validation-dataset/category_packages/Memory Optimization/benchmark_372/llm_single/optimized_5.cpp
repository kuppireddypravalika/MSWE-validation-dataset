#include "minicurl.hpp"

// Optimized implementation avoids unnecessary copy
std::string MiniCurl::getURL(size_t size) {
    d_data.clear();
    perform_request(size);
    return std::move(d_data); // move instead of copy
}

std::string getURL(MiniCurl& curl, size_t size) {
    return curl.getURL(size);
}
