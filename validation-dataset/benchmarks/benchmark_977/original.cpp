#include "converter.hpp"
#include <cstring>

// Original implementation with manual ASCII conversion before calling the UTF-8 helper
void convertFromAscii(const char* a, int len, char16_t*& out) {
    if (len == -1) {
        if (!a)
            return;
        while (*a && static_cast<unsigned char>(*a) < 0x80)
            *out++ = static_cast<char16_t>(*a++);
        if (!*a)
            return;
        len = int(strlen(a));
    } else {
        int i;
        for (i = 0; i < len && static_cast<unsigned char>(a[i]) < 0x80; ++i)
            *out++ = static_cast<char16_t>(a[i]);
        if (i == len)
            return;
        a += i;
        len -= i;
    }

    // Remaining characters handled via UTF-8 conversion
    out = fastConvertToUnicode(out, a, len);
}
