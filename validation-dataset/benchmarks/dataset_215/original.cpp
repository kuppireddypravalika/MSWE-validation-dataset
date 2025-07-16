#include <cstdint>
#include <cstring>

uint64_t run(size_t repeat) {
    constexpr int LEN = 3; // "C:\"
    constexpr int NUM = 5;
    static char drives[(LEN + 1) * NUM + 1];
    static bool init = false;
    if (!init) {
        const char letters[NUM] = {'C','D','E','F','G'};
        for (int i = 0; i < NUM; ++i) {
            int idx = i * (LEN + 1);
            drives[idx] = letters[i];
            drives[idx+1] = ':';
            drives[idx+2] = '\\';
            drives[idx+LEN] = '\0';
        }
        drives[NUM*(LEN+1)]='\0';
        init = true;
    }
    uint64_t sum = 0;
    for (size_t r = 0; r < repeat; ++r) {
        const char* drive = drives;
        while (*drive) {
            int offset = std::strlen(drive) + 1; // computed each iteration
            sum += static_cast<unsigned>(drive[0]);
            drive += offset;
        }
    }
    return sum;
}
