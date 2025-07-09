#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

constexpr int nLeaderSize = 24;
constexpr char DDF_FIELD_TERMINATOR = 0x1e;

int read_record(FILE* fp, int recLength) {
    char leader[nLeaderSize];
    fread(leader, 1, nLeaderSize, fp);

    int nDataSize = recLength - nLeaderSize;
    std::vector<char> pachData(static_cast<size_t>(nDataSize) + 1);

    // Read initial chunk of data
    size_t bytesRead = fread(pachData.data(), 1, nDataSize, fp);

    // Loop until we find the DDF_FIELD_TERMINATOR
    while (bytesRead == nDataSize || (pachData[bytesRead - 1] != DDF_FIELD_TERMINATOR || (bytesRead >= 2 && pachData[bytesRead - 2] != DDF_FIELD_TERMINATOR))) {
        nDataSize++;
        pachData.resize(static_cast<size_t>(nDataSize) + 1);
        pachData[nDataSize] = '\0';
        bytesRead += fread(pachData.data() + bytesRead, 1, 1, fp);
    }

    long long sum = 0;
    for (size_t i = 0; i < bytesRead; ++i) sum += pachData[i];
    return static_cast<int>(sum);
}

// Explicit template instantiations