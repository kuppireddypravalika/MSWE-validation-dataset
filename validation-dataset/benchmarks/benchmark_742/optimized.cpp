#include <cstdint>
#include <cstring>
#include <vector>

// Optimized version of the secure area check.
// Reads two elements per iteration using pointer arithmetic and uintptr_t
// in order to check 64 bits at a time on 64-bit systems.

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

const char *check_secure_area(const std::vector<uint8_t> &rom)
{
    uint32_t secure_area[4];
    std::memcpy(secure_area, &rom[0x4000], sizeof(secure_area));

    const char *secType = nullptr;
    if (secure_area[0] == 0xE7FFDEFF && secure_area[1] == 0xE7FFDEFF) {
        secType = "Decrypted";
    } else {
        uintptr_t blank_area[0x3000 / sizeof(uintptr_t)];
        std::memcpy(blank_area, &rom[0x1000], sizeof(blank_area));

        const uintptr_t *const end = &blank_area[ARRAY_SIZE(blank_area) - 1];
        for (const uintptr_t *p = blank_area; p < end; p += 2) {
            if (p[0] != 0 || p[1] != 0) {
                secType = "Mask ROM";
                break;
            }
        }

        if (!secType)
            secType = "Encrypted";
    }

    return secType;
}


