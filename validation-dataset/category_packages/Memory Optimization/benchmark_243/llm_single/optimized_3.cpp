#include "ByteArray.h"

void ba_copy(const char* buf, size_t len, ByteArray& data) {
    if (len > 0) {
        data.assign(buf, len); // Directly assign to existing ByteArray
    } else {
        data.assign(nullptr, 0); // Clear or assign empty if len is 0
    }
}
