#include "ByteArray.h"

void ba_copy(const char* buf, size_t len, ByteArray& data) {
    if (len > 0) {
        data.assign(buf, len); // assign directly to avoid temporary
    }
}
