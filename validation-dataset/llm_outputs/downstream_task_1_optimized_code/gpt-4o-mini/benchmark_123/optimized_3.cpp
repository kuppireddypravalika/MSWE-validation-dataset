#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

struct SmallString {
    std::string data;
    void append(const std::string& s) { data += s; }
    const char* c_str() const { return data.c_str(); }
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
    SmallString TmpStr;
    TmpStr.append(input);
    size_t sum = 0;
    if (!TmpStr.empty()) {
        const char* cstr = TmpStr.c_str();
        sum += DummySetCrashLogMessage(cstr);
        char* dup = DummyDuplicate(cstr);
        sum += std::strlen(dup);
        std::free(dup);
        sum += TmpStr.data.size();
    }
    return sum;
}

// Explicit template instantiation statements, if any, should be added below.
