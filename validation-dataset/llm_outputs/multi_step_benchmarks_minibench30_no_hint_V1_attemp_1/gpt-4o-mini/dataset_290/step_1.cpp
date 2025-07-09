#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>

constexpr int nLeaderSize = 24;
constexpr char DDF_FIELD_TERMINATOR = 0x1e;

int read_record(FILE* fp, int recLength) {
    char leader[nLeaderSize];
    fread(leader, 1, nLeaderSize, fp);

    int nDataSize = recLength - nLeaderSize;
    char* pachData = static_cast<char*>(std::malloc(nDataSize + 1));
    if (!pachData) return -1;
    pachData[nDataSize] = '\0';

    fread(pachData, 1, nDataSize, fp);

    long long sum = 0;

    // Use a single while loop to read until we have the terminator
    while (true) {
        if (pachData[nDataSize - 1] == DDF_FIELD_TERMINATOR && 
            (nDataSize < 2 || pachData[nDataSize - 2] == DDF_FIELD_TERMINATOR)) {
            break;
        }
        nDataSize++;
        char* newData = static_cast<char*>(std::realloc(pachData, nDataSize + 1));
        if (!newData) {
            std::free(pachData);
            return -1; // Handle the error if realloc fails
        }
        pachData = newData;
        fread(pachData + nDataSize - 1, 1, 1, fp);
        pachData[nDataSize] = '\0';
    }

    for (int i = 0; i < nDataSize; ++i) sum += pachData[i];
    std::free(pachData);
    return static_cast<int>(sum);
}

// Explicit template instantiations
