#include <cstdint>
#include <cstring>
#include <vector>

const char *check_secure_area(const std::vector<uint8_t> &rom)
{
    // Read the first 16 bytes of the secure area.
    const uint32_t *secure_area = reinterpret_cast<const uint32_t*>(&rom[0x4000]);
    
    if (secure_area[0] == 0xE7FFDEFF && secure_area[1] == 0xE7FFDEFF) {
        // Decrypted dump.
        return "Decrypted";
    } 
    
    // Read 0x3000 bytes starting at 0x1000 and verify they are all zero.
    const uint32_t *blank_area = reinterpret_cast<const uint32_t*>(&rom[0x1000]);
    size_t size = 0x3000 / sizeof(uint32_t);
    
    for (size_t i = 0; i < size; i++) {
        if (blank_area[i] != 0) {
            return "Mask ROM";
        }
    }

    return "Encrypted";
}