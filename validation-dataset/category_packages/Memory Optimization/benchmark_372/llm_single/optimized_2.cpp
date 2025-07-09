#include "minicurl.hpp"

// Optimized implementation avoids unnecessary copy
std::string MiniCurl::getURL(size_t size) {
    d_data.clear();
    perform_request(size);
    std::string ret;
    ret.swap(d_data); // Move data instead of copy
    d_data.clear();
    return ret;
}

std::string getURL(MiniCurl& curl, size_t size) {
    return curl.getURL(size);
}
