#include <cstdint>
#include <cstring>
#include <vector>

// Simplified stand-in for NintendoDSPrivate::checkNDSSecureArea().
// Scans a region of the ROM image to determine if the secure area is blank.

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

const char *check_secure_area(const std::vector<uint8_t> &rom)
{
    // Read the first 16 bytes of the secure area.
    uint32_t secure_area[4];
    std::memcpy(secure_area, &rom[0x4000], sizeof(secure_area));

    const char *secType = nullptr;
    if (secure_area[0] == 0xE7FFDEFF && secure_area[1] == 0xE7FFDEFF) {
        // Decrypted dump.
        secType = "Decrypted";
    } else {
        // Read 0x3000 bytes starting at 0x1000 and verify they are all zero.
        uint32_t blank_area[0x3000 / 4];
        std::memcpy(blank_area, &rom[0x1000], sizeof(blank_area));

        for (int i = ARRAY_SIZE(blank_area) - 1; i >= 0; i--) {
            if (blank_area[i] != 0) {
                secType = "Mask ROM";
                break;
            }
        }

        if (!secType)
            secType = "Encrypted";
    }

    return secType;
}


