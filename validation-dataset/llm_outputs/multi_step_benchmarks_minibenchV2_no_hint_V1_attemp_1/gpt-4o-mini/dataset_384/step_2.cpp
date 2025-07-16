#include <cctype>
#include <cstddef>
#include <cstring>
extern "C" int bstr_util_mem_index_of_mem_nocasenorzero(const void *data1,size_t len1,const void *data2,size_t len2){
    const unsigned char* d1 = static_cast<const unsigned char*>(data1);
    const unsigned char* d2 = static_cast<const unsigned char*>(data2);
    const unsigned char* d1_end = d1 + len1;
    const unsigned char* d2_end = d2 + len2;

    size_t i, j;
    for(i = 0; d1 + i < d1_end; i++) {
        const unsigned char* k = d1 + i;
        for(j = 0; j < len2 && k < d1_end; j++, k++) {
            if(*k == 0) {
                j--;
                continue;
            }
            if(toupper(*k) != toupper(d2[j])) break;
        }
        if(j == len2) return static_cast<int>(i);
    }
    return -1;
}

// Explicit template instantiations if present
