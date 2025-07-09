#include "converter.hpp"
#include <cstring>

// Optimized implementation calling the UTF-8 helper directly
void convertFromAscii(const char* a, int len, char16_t*& out) {
    if (len == -1) {
        if (!a)
            return;
        len = int(strlen(a));
    }
    out = fastConvertToUnicode(out, a, len);
}
