#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <vector>

constexpr int nLeaderSize = 24;
constexpr char DDF_FIELD_TERMINATOR = 0x1e;

int read_record(FILE* fp, int recLength) {
    char leader[nLeaderSize];
    fread(leader, 1, nLeaderSize, fp);

    int nDataSize = recLength - nLeaderSize;
    char* pachData = static_cast<char*>(std::malloc(nDataSize + 1));
    if (!pachData) return -1;
    pachData[nDataSize] = '\0';

    std::size_t totalRead = fread(pachData, 1, nDataSize, fp);

    while (pachData[nDataSize - 1] != DDF_FIELD_TERMINATOR || (nDataSize >= 2 && pachData[nDataSize - 2] != DDF_FIELD_TERMINATOR)) {
        nDataSize++;
        char* newData = static_cast<char*>(std::realloc(pachData, nDataSize + 1));
        if (!newData) {
            std::free(pachData);
            return -1;
        }
        pachData = newData;
        totalRead += fread(pachData + totalRead, 1, 1, fp);
        pachData[nDataSize] = '\0';
    }

    long long sum = 0;
    for (int i = 0; i < nDataSize; ++i) sum += pachData[i];
    std::free(pachData);
    return static_cast<int>(sum);
}

// Explicit template instantiations