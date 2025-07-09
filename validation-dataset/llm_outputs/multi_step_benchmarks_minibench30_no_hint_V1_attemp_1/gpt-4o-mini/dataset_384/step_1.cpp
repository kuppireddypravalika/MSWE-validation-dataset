#include <cctype>
#include <cstddef>
#include <cstring>
extern "C" int bstr_util_mem_index_of_mem_nocasenorzero(const void *data1,size_t len1,const void *data2,size_t len2){
    const unsigned char* d1 = static_cast<const unsigned char*>(data1);
    const unsigned char* d2 = static_cast<const unsigned char*>(data2);
    size_t i, j;

    for (i = 0; i < len1; ++i) {
        size_t k = i;
        j = 0;
        while (j < len2 && k < len1) {
            if (d1[k] == 0) {
                ++k; // Skip the zero character and continue
                continue;
            }
            if (std::toupper(d1[k]) != std::toupper(d2[j])) break;
            ++j;
            ++k;
        }
        if (j == len2) return i;
    }
    return -1;
}

