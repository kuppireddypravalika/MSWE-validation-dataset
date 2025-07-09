#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

struct SmallString {
    std::string data;
    void append(const std::string& s) { data += s; }
    const char* c_str() const { return data.c_str(); }
    std::string str() const { return data; }
    bool empty() const { return data.empty(); }
};

static size_t DummySetCrashLogMessage(const char* msg) {
    // Simulate external API that immediately consumes the message
    volatile size_t len = std::strlen(msg);
    return len;
}

static char* DummyDuplicate(const char* msg) {
    return ::strdup(msg);
}

size_t handler(const std::string& input) {
    if (input.empty()) return 0; // Early exit if input is empty

    SmallString TmpStr;
    TmpStr.append(input);
    size_t sum = 0;

    sum += DummySetCrashLogMessage(TmpStr.c_str());
    char* dup = DummyDuplicate(TmpStr.c_str());
    sum += std::strlen(dup);
    std::free(dup);
    sum += TmpStr.str().size();

    return sum;
}

