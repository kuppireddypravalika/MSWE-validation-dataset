#include <cctype>
#include <cstddef>
#include <cstring>

extern "C" int bstr_util_mem_index_of_mem_nocasenorzero(const void *data1, size_t len1, const void *data2, size_t len2) {
    const unsigned char* d1 = static_cast<const unsigned char*>(data1);
    const unsigned char* d2 = static_cast<const unsigned char*>(data2);

    // Use pointers to traverse through the data
    const unsigned char* p1 = d1;
    const unsigned char* p2 = d2;
    for (size_t i = 0; i < len1; ++i) {
        const unsigned char* start = p1 + i;
        const unsigned char* k = start;
        size_t j = 0;

        while (j < len2 && (k < d1 + len1)) {
            // Skip zero characters in the main data
            if (*k == 0) {
                ++k; // Skip zero character
                continue;
            }
            if (std::toupper(*k) != std::toupper(p2[j])) {
                break;
            }
            ++j;
            ++k;
        }

        if (j == len2) return static_cast<int>(i);
    }
    return -1;
}

// Explicit template instantiations
// None needed for this function