#include <cstdint>
#include <unordered_map>
#include <string>
#include <cstring>

uint64_t run(size_t repeat) {
    volatile uint64_t sum = 0;
    constexpr int LEN = 32;
    constexpr int NUM = 5;
    static char drives[(LEN + 1) * NUM + 1];
    static bool init = false;
    if (!init) {
        const char letters[NUM] = {'C','D','E','F','G'};
        for (int i = 0; i < NUM; ++i) {
            size_t idx = i * (LEN + 1);
            drives[idx] = letters[i];
            drives[idx+1] = ':';
            drives[idx+2] = '\\';
            for (int j = 3; j < LEN; ++j) drives[idx+j] = 'a' + i;
            drives[idx+LEN] = '\0';
        }
        drives[NUM*(LEN+1)]='\0';
        init = true;
    }
    for (size_t r = 0; r < repeat; ++r) {
        std::unordered_map<std::string, std::string> labels;
        const char* drive = drives;
        int offset = std::strlen(drive) + 1; // computed once before loop
        while (*drive) {
            std::string volumename(drive);
            if (!volumename.empty()) {
                labels[volumename] = std::string(drive).substr(0, 2);
            }
            drive += offset;
        }
        for (auto& p : labels) sum += p.second.size();
    }
    return sum;
}
