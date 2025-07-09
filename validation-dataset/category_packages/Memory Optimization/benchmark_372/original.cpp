#include "minicurl.hpp"

// Original implementation copies the internal buffer
std::string MiniCurl::getURL(size_t size) {
    d_data.clear();
    perform_request(size);
    std::string ret = d_data; // copy
    d_data.clear();
    return ret;
}

std::string getURL(MiniCurl& curl, size_t size) {
    return curl.getURL(size);
}
